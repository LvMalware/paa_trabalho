#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/graph.h"
#include "lib/kruskal.h"
#include "lib/dijkstra.h"

void print(const char *);

graph_t * read_graph ();

graph_t * random_graph (int);

int node_index(graph_t *, const char *);

int silent = 0;

int
main (int argc, char *argv[])
{

    /* allow the user to pass a '-s' switch to turn off the interactivity */
    if (argc > 1)
        silent = (strcmp(argv[1], "-s") == 0);
    
    /* print() will only print the text to the screen if silent is false */
    print("=== Kruskal and Dijkstra algorithms ===\n");
    print("Implementation by: Lucas V. Araujo, Matheus H. de Souza Fontenele,\n"
           "Matheus H. Miranda and Odilon F. Damasceno Neto\n\n");

    /* we only have one graph and one mst at a time */
    graph_t *g = NULL, *mst = NULL;
    
    int opt = 1;

    char buf[100];
    
    int i, count, from, to, *path;
    unsigned cost;
    
    while (1)
    {
        print("Options: \n\n");
        print("1) Create a new graph\n");
        print("2) Show the edges for the last graph\n");
        print("3) Find and show the MST for the last graph\n");
        print("4) Find the minimum path between two nodes in the last graph\n");
        print("0) Exit\n\n");
        print("> ");

        /* using fgets and atoi, we avoid some bugs with scanf input parsing */
        while (fgets(buf, 100, stdin) != buf);

        opt = atoi(buf);
        /* option 0 closes the program */
        if (opt == 0)
            break;

        switch (opt)
        {
            case 1:
                /* get a new graph */
                if (g)
                    graph_free(g);
                g = read_graph();
                if (mst)
                    graph_free(mst);
                mst = NULL;
                break;
            case 2:
                /* show the edges for the current graph */
                if (!g)
                {
                    printf("No graph\n");
                    break;
                }
                printf("\n");
                graph_show(g);
                printf("\n");
                printf("Total cost: %d\n\n", g->total);
                break;
            case 3:
                /* find and show the edges for the MST of the current graph */
                if (!g)
                {
                    printf("No graph\n");
                    break;
                }
                if (!mst)
                    mst = kruskal_mst(g);
                printf("\n");
                graph_show(mst);
                printf("\n");
                printf("Total cost: %d\n\n", mst->total);
                break;
            case 4:
                /* fint the minimal path between two nodes on the graph */
                if (!g)
                {
                    printf("No graph\n");
                    break;
                }
                
                /* read the origin node */
                print("From node: ");
                while (fgets(buf, 100, stdin) != buf);
                /* remove the newline */
                buf[strcspn(buf, "\n")] = '\0';
                /* get the index of the node by its name */
                from = node_index(g, buf); //atoi(buf);
                /* read the destination node */
                print("To node: ");
                while (fgets(buf, 100, stdin) != buf);
                /* remove the newline */
                buf[strcspn(buf, "\n")] = '\0';
                /* get the index of the node by its name */
                to = node_index(g, buf);
                /* using Dijkstra algorithm, find the minimum cost and path */
                cost = dijkstra(g, from, to, &count, &path);
                /* print the cost and path found */
                printf("Minimum cost: %u\n", cost);
                printf("Path: ");
                for (i = 0; i < count; i ++)
                    printf("%s -> ", g->names[path[i]]);
                printf("\b\b\b   \n");
                /* we don't need the path after that, so we can free it */
                free(path);
        }
    }
    
    /* free the graph and mst when exiting */
    if (g)
        graph_free(g);
    
    if (mst)
        graph_free(mst);
    return 0;
}

graph_t *
read_graph ()
{
    /* read some options from the user to generate a new graph */
    print("==== Adding a new graph! ====\n");

    char buf[100];
    
    int i, ncount = 0;

    /* get the number of nodes that the user wishes */
    print("Number of nodes: ");
    /* loop with fgets to ensure we are reading a valid value */
    while (fgets(buf, 100, stdin))
    {
        ncount = atoi(buf);
        if (ncount < 1)
            print("Invalid number of nodes! Try again\nNumber of nodes: ");
        else
            break;
    }
    

    print("Do you want to type in each edge? (Y/N) ");
    /* again, loop with fgets to read the user input */    
    while(fgets(buf, 100, stdin) == NULL);
    /* if the user don't want to enter each edge, we generate them randomly and
     * return the resulting graph
     */
    if (buf[0] == 'N' || buf[0] == 'n')
        return random_graph(ncount);
    
    /* the user will type the edges by hand (that will be a lot of work!) */
    graph_t *g = graph_new(ncount);
    /* give the nodes pretty alphabetical names if possible */
    if (ncount < 26)
    {
        char name[2] = "A";
        for (i = 0; i < ncount; i ++)
        {
            name[0] = 'A' + i;
            graph_name_node(g, i, name);
        }
    }
    
    int from, to, cost;
    print("Type the values for each edge in one line, separated by single "
           "spaces:\n");
    print("FROM   TO   COST\n");
    /* loop with fgets reading the input of the user */    
    while (fgets(buf, 100, stdin) != NULL)
    {
        if (strlen(buf) < 3)
            break;
        /* parses the user input and set the values of the nodes and cost for
         * each of the edges.
         */
        sscanf(buf, "%d %d %d", &from, &to, &cost);
        /* add the edge to the graph */
        graph_edge(g, from, to, cost);
    }

    print("Done.\n");
    /* return the graph */
    return g;
}

graph_t *
random_graph (int ncount)
{
    /* generate a random graph with the given number of nodes */
    
    /* create a new graph_t structure */
    graph_t *g = graph_new(ncount);
    
    /* seed the PRNG with the current time XOR the number of nodes */
    srandom(time(NULL) ^ ncount);
    

    int i;
    /* get the number of edges of the generated graph */
    int edges = (ncount > 5) ? ncount + ncount / 2 : ncount - 1;
    
    /* NOTE: the above code is based on an assumption made during implementation
     * to allow the generation of a graph with a suitable number of edges which
     * will have only one (two-way) edge between each pair of connected nodes,
     * where at the same time, every node will have at least one connection to
     * another node. As you will see, if we didn't limit the number of edges as
     * was we did, the code to generate the edges randomly could go into an
     * infinity loop (sic!)
     */
    
    char name[2] = "";
    /* if we only have up to 26 nodes, we can name each of them as letters for
     * easy identification.
     */
    if (ncount < 26)
    {
        for (i = 0; i < ncount; i ++)
        {
            /* 65 is the ASCII representation of 'A'. Adding a numeric value to
             * between 0 and 25 to it will result in letters between 'A' and 'Z'
             */
            name[0] = 65 + i;
            /* set the name of the node i */
            graph_name_node (g, i, name);
        }
    }
    
    int j, n = 0;

    /* generate the edges randomly */
    for (i = 0; i < edges; i ++)
    {
        do {
            /* get a node at random and stores it at j */
            j = random() % ncount;
            /* loop if j already is connected to node n */
        } while (graph_has_edge (g, n, j));
        /* create an edge between j and n with a random cost between 0 and 20 */
        graph_edge (g, n, j, 1 + random() % 20);
        /* get the next node at random and stores it at n */
        n = (n + 1) % ncount;
    }
    
    /* return the graph */
    return g;
}

void
print(const char *msg)
{
    if (!silent)
        printf("%s", msg);
}

int
node_index(graph_t *g, const char *name)
{
    /* returns the index of a node given it's name */
    
    int i, index;
    /* if the nodes have alphabetical names */
    if (strlen(name) == 1 && g->count <= 26)
        index = (int) name[0] - 65;
    /* if they have the standard qXX names */
    else if (name[0] == 'q' && name[1] >= 48 && name[1] <= 57)
        sscanf(name, "q%d", &index);
    /* in any other case */
    else
    {
        for (i = 0; i < g->count; i ++)
        {
            if (strcmp(name, g->names[i]) == 0)
            {
               index = i;
               break;
            }
        }
    }

    return index;
}

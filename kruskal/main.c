#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/graph.h"
#include "lib/kruskal.h"

void print(const char *);

graph_t * read_graph ();

graph_t * random_graph (int, int);

int silent = 0;

int
main (int argc, char *argv[])
{

    if (argc > 1)
        silent = (strcmp(argv[1], "-s") == 0);
    
    print("=== Kruskal algorithm for Minimal SubTree (MST) ===\n");
    print("Implementated by: Lucas V. Araujo, Matheus H. de Souza Fontenele,\n"
           "Odilon F. Damasceno Neto and Matheus H. Miranda\n\n");

    graph_t *g = NULL, *mst = NULL;
    
    int opt = 1;

    char buf[100];
    
    while (1)
    {
        print("Options: \n\n");
        print("1) Create a new graph\n");
        print("2) Show the edges for the last graph\n");
        print("3) Find and show the MST for the last graph\n");
        print("0) Exit\n\n");
        print("> ");

        while (fgets(buf, 100, stdin) != buf);

        opt = atoi(buf);
        if (opt == 0)
            break;

        switch (opt)
        {
            case 1:
                if (g)
                    graph_free(g);
                g = read_graph();
                if (mst)
                    graph_free(mst);
                mst = NULL;
                break;
            case 2:
                printf("\n");
                graph_show(g);
                printf("\n");
                printf("Total cost: %d\n\n", g->total);
                break;
            case 3:
                if (!mst)
                    mst = kruskal_mst(g);
                printf("\n");
                graph_show(mst);
                printf("\n");
                printf("Total cost: %d\n\n", mst->total);
        }
    }
    if (g)
        graph_free(g);
    
    if (mst)
        graph_free(mst);
    return 0;
}

graph_t *
read_graph ()
{
    printf("==== Adding a new graph! ====\n");

    char buf[100];
    
    int i, ncount = 0;

    print("Number of nodes: ");
    while (fgets(buf, 100, stdin))
    {
        ncount = atoi(buf);
        if (ncount < 1)
            print("Invalid number of nodes! Try again\nNumber of nodes: ");
        else
            break;
    }
    
    print("Do you want to type in each edge? (Y/N) ");
    
    while(fgets(buf, 100, stdin) == NULL);

    if (buf[0] == 'N' || buf[0] == 'n')
        return random_graph(ncount, 0);

    graph_t *g = graph_new(ncount, 0);
    
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
    while (fgets(buf, 100, stdin) != NULL)
    {
        if (strlen(buf) < 3)
            break;
        sscanf(buf, "%d %d %d", &from, &to, &cost);
        graph_edge(g, from, to, cost);
    }

    print("Done.\n");
    
    return g;
}

graph_t *
random_graph (int ncount, int directed)
{
    graph_t *g = graph_new(ncount, directed);

    int *v = (int *) &g;
    
    srandom(*v ^ ncount);
    
    int i, edges = (ncount > 3) ? ncount + ncount / 2 : 3;

    char name[2] = "";
    if (ncount < 26)
    {
        for (i = 0; i < ncount; i ++)
        {
            name[0] = 65 + i;
            graph_name_node (g, i, name);
        }
    }
    
    int j, n = 0;

    for (i = 0; i < edges; i ++)
    {
        do {
            j = random() % ncount;
        } while (graph_has_edge (g, n, j));
        graph_edge (g, n, j, random() % 20);
        n = (j + i) % ncount;
    }
    return g;
}

void
print(const char *msg)
{
    if (!silent)
        printf("%s", msg);
}
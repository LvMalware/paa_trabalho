#include "graph.h"

graph_t *
graph_new (int count)
{
    /* allocates and initializes memory for a new graph_t structure */
    graph_t *g = calloc(1, sizeof(graph_t));

    g->nodes = (int **) calloc(count, sizeof(int *));
    g->count = count;
    g->names = (char **) calloc(count + 1, sizeof(char *));
    g->edges = 0;
    g->total = 0;

    int i;
    char name[10];

    /* NOTE: we are using an adjacence matrix to represent the graph edges */

    for (i = 0; i < count; i ++)
    {
        /* allocates a line for the node i */
        g->nodes[i] = malloc(count * sizeof(int));
        /* initializes every position of the line i to -1
         * NOTE: the value at line x, column y represents the cost of the edge
         * between node x and y. If there is no edge, this value is -1
         */
        memset(g->nodes[i], -1, count * sizeof(int));
        /* the node has always an 'edge' to itself with cost 0 */
        g->nodes[i][i] = 0;
        /* lets give a name to the node for easy identification when printing */
        sprintf(name, "q%d", i);
        graph_name_node(g, i, name);
    }
    
    return g;
}

void
graph_name_node (graph_t *g, int node, const char *name)
{
    /* sets the name of a node to an specified value */
    
    /* free the current char array that holds its name */
    if (g->names[node])
        free(g->names[node]);

    /* duplicates the char *name array setting it to be the new name */
    g->names[node] = strdup(name);
}

void
graph_edge (graph_t *g, int from, int to, int cost)
{
    /* adds an edge between the two specified nodes with the given cost */
    g->nodes[from][to] = cost;
    /* since we are dealing with undirected graphs, there is a two way edge
     * between them, which we represent by having two edges on the matrix
     */
    g->nodes[to][from] = cost;
    /* updates the total cost of the graph */
    g->total += cost;
    /* updates the number of edges */
    g->edges ++;
}

void
graph_free (graph_t *g)
{
    /* desallocates the memory for a given graph_t structure */
    
    int i;
    for (i = 0; i < g->count; i ++)
    {
        /* we must free the name and the line for each node */
        if (g->nodes[i])
            free(g->nodes[i]);
        if (g->names[i])
            free(g->names[i]);
    }
    /* and free the arrays that held their addresses */
    free(g->names);
    free(g->nodes);
    /* finally we can free the graph_t structure */
    free(g);
}

void
graph_show (graph_t *g)
{
    /* prints the edges that form a graph */
    
    int i, j;
    
    for (i = 0; i < g->count; i ++)
    {
        for (j = 0; j < g->count; j ++)
        {
            /* we don't need to show the connection from a node to itself since
             * it is an implicit property of graphs.
             */
            if (i != j && g->nodes[i][j] > 0)
            {
                /* print the nodes and the cost between them */
                printf("%s ---- %d ----> %s\n", g->names[i],
                       g->nodes[i][j], g->names[j]);
                /* once we have shown the edge between i and j, we don't need
                 * to display it between j and i (we already know it is a two
                 * way edge), so we remove the other way and restore it later
                 */
                 g->nodes[j][i] = -1;
            }
        }
    }
    
    
    /* now we loop through each edge and restore their counterpart */
    for (i = 0; i < g->count; i++)
    {
        for (j = 0; j < g->count; j ++)
            g->nodes[j][i] = g->nodes[i][j];
    }
}

int
graph_has_edge(graph_t *g, int from, int to)
{
    /* checks if there is an edge between to nodes on the graph */
    if (!g || !g->nodes)
        return 0;
    return (g->nodes[from][to] >= 0) || (g->nodes[to][from] >= 0);
}

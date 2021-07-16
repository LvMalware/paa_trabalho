#include "graph.h"

graph_t *
graph_new (int count, int directed)
{
    graph_t *g = calloc(1, sizeof(graph_t));
    
    g->nodes = (int **) calloc(count, sizeof(int *));
    g->count = count;
    g->names = (char **) calloc(count + 1, sizeof(char *));
    g->edges = 0;
    g->total = 0;

    g->directed = directed;

    int i;
    char name[10];

    for (i = 0; i < count; i ++)
    {
        g->nodes[i] = malloc(count * sizeof(int));
        memset(g->nodes[i], -1, count * sizeof(int));
        g->nodes[i][i] = 0;
        sprintf(name, "q%d", i); 
        graph_name_node(g, i, name);
    }
    
    return g;
}

void
graph_name_node (graph_t *g, int node, const char *name)
{
    if (g->names[node])
        free(g->names[node]);
    g->names[node] = strdup(name);
}

void
graph_edge (graph_t *g, int from, int to, int cost)
{
    g->nodes[from][to] = cost;
    if (!g->directed)
        g->nodes[to][from] = cost;
    g->total += cost;
    g->edges ++;
}

void
graph_free (graph_t *g)
{
    int i;
    for (i = 0; i < g->count; i ++)
    {
        if (g->nodes[i])
            free(g->nodes[i]);
        if (g->names[i])
            free(g->names[i]);
    }
    free(g->names);
    free(g->nodes);
    free(g);
}

int
ndigits(int number)
{
    int n = 1;
    while (number > 0)
    {
        n += 1;
        number /= 10;
    }
    return n;
}

void
graph_show (graph_t *g)
{
    int i, j;

    int *shown = calloc(g->count * g->count, sizeof(int));
    
    for (i = 0; i < g->count; i ++)
    {
        for (j = 0; j < g->count; j ++)
        {
            if (i == j)
                continue;
            else if (g->nodes[i][j] > 0)
            {
                int x = i * g->count + j;
                int y = j * g->count + i;
                if (shown[x] == 1)
                    continue;
                printf("%s ---- %d ----> %s\n", g->names[i],
                       g->nodes[i][j], g->names[j]);
                shown[x] = shown[y] = 1;
            }
        }
    }
    
    free(shown);
}

int
graph_has_edge(graph_t *g, int from, int to)
{
    if (!g || !g->nodes)
        return 0;
    return g->nodes[from][to] >= 0;
}
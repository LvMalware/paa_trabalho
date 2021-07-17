#include "kruskal.h"

int
kfind (int *subset, int v)
{
    while (subset[v] != -1)
        v = subset[v];
    return v;
}

void
kunion (int *subset, int a, int b)
{
    int a_set = kfind(subset, a);
    int b_set = kfind(subset, b);
    subset[a_set] = b_set;
}

int
kruskal_circular_check (int *subset, int x, int y)
{
    int i = kfind(subset, x);
    int j = kfind(subset, y);

    if (i == j)
        return 0;
    kunion(subset, i, j);
    return 1;
}

int
kruskal_sort_cmp (const void *a, const void *b)
{
    edge_t *x = (edge_t *) a;
    edge_t *y = (edge_t *) b;
    return x->cost - y->cost;
}

int
added_edge_check(edge_t *e, int s, int from, int to)
{
    int i;
    for (i = 0; i < s; i ++)
    {
        if ((e[i].orig == from && e[i].dest == to) ||
            (e[i].dest == from && e[i].orig == to))
            return 1;
    }
    return 0;
}

graph_t *
kruskal_mst (graph_t *g)
{
    edge_t *edges = malloc(g->edges * sizeof(edge_t));

    graph_t *mst = graph_new(g->count);
    
    int *subset = malloc(g->count * sizeof(int));
    memset(subset, -1, g->count * sizeof(int));
    
    int i, j, k = 0;

    for (i = 0; i < g->count; i ++)
    {
        graph_name_node(mst, i, g->names[i]);
        for (j = 0; j < g->count; j ++)
        {
            if (g->nodes[i][j] > 0)
            {
                if (added_edge_check(edges, k, i, j))
                    continue;
                edges[k].orig = i;
                edges[k].dest = j;
                edges[k].cost = g->nodes[i][j];
                k += 1;
            }
        }
    }
    
    sort(edges, k, sizeof(edge_t), kruskal_sort_cmp);
    
    for (i = 0; i < k; i ++)
        if (kruskal_circular_check (subset, edges[i].orig, edges[i].dest))
            graph_edge (mst, edges[i].orig, edges[i].dest, edges[i].cost);
    free(edges);   
    return mst;
}

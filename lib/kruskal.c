#include "kruskal.h"

int
kfind (int *subset, int v)
{
    /* Union-Find find: return the subset which v is part of  */
    while (subset[v] != -1)
        v = subset[v];
    return v;
}

void
kunion (int *subset, int a, int b)
{
    /* Union-Find union: unite the two nodes into the same subset */
    int a_set = kfind(subset, a);
    int b_set = kfind(subset, b);
    subset[a_set] = b_set;
}

int
kruskal_circular_check (int *subset, int x, int y)
{
    /* checks for circles in the sub-graph */
    int i = kfind(subset, x);
    int j = kfind(subset, y);

    /* if there is a circle, this edge won't be used, so we return 0 */

    if (i == j)
        return 0;
    /* if there is no circle, unite the two nodes into the same subset */
    kunion(subset, i, j);
    /* return 1 to indicate this is a valid edge */
    return 1;
}

int
kruskal_sort_cmp (const void *a, const void *b)
{
    /* compares the two input nodes based on their costs */
    edge_t *x = (edge_t *) a;
    edge_t *y = (edge_t *) b;
    /* returns 0 if they are equal, a negative number if a is lesser than b
     * and a positive number otherwise
     */
    return x->cost - y->cost;
}

graph_t *
kruskal_mst (graph_t *g)
{
    /* Kruskal algorithm: finds the Minimal SubTree (MST) for a given graph */
    
    /* allocate an array that will hold the edges from the original graph */
    edge_t *edges = malloc(g->edges * sizeof(edge_t));

    /* create a new graph_t structure that will be returned as result */
    graph_t *mst = graph_new(g->count);
    
    /* allocate a subset array that will be used in the Union-Find algorithm to
     * detect circles
     */
    int *subset = malloc(g->count * sizeof(int));
    /* sets each position of the subset array to -1 indicating that their subset
     * is unknown at the time.
     */
    memset(subset, -1, g->count * sizeof(int));
    
    int i, j, k = 0;
    
    /* populate the edges list */
    for (i = 0; i < g->count; i ++)
    {
        /* replicate the same node name on the MST */
        graph_name_node(mst, i, g->names[i]);
        for (j = 0; j < g->count; j ++)
        {
            if (g->nodes[i][j] > 0)
            {
                edges[k].orig = i;
                edges[k].dest = j;
                edges[k].cost = g->nodes[i][j];
                /* avoid inserting duplicated edges by removing each of them
                 * from the original graph after adding it to the array.
                 */
                g->nodes[i][j] = g->nodes[j][i] = -1;
                /* at the end, k will hold the number of valid edges */
                k += 1;
            }
        }
    }
    
    /* sort the edges based on their cost */
    sort(edges, k, sizeof(edge_t), kruskal_sort_cmp);
 
    /* let's reset number of edges and the total cost of the graph to 0, since
     * we will restore the previous edges and it will mess the these values if
     * we don't
     */
    g->total = g->edges = 0;
    /* iterates over the now sorted edges list */
    for (i = 0; i < k; i ++)
    {
        /* restore the edges on the original graph */
        graph_edge(g, edges[i].orig, edges[i].dest, edges[i].cost);
        /* add the node to the MST if it doesn't create a circle */
        if (kruskal_circular_check (subset, edges[i].orig, edges[i].dest))
            graph_edge (mst, edges[i].orig, edges[i].dest, edges[i].cost);
    }
    
    /* mst now holds a minimal subtree for the graph g */
    
    /* free the edges array */
    free(edges);
    
    return mst;
}

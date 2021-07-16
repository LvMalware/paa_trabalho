#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/graph.h"
#include "lib/kruskal.h"

graph_t * random_graph (int, int);

int
main()
{

    graph_t *g = random_graph(10, 0);
    graph_show(g);

    printf("\n\nTotal cost: %d\n\n", g->total);
    
    graph_t *mst = kruskal_mst (g);

    graph_free (g);
    
    graph_show(mst);

    printf("\n\nTotal cost: %d\n\n", mst->total);

    graph_free (mst);
    
    return (0);
}

graph_t *
random_graph (int ncount, int directed)
{
    graph_t *g = graph_new(ncount, directed);

    srand(time(NULL) ^ ncount);
    
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

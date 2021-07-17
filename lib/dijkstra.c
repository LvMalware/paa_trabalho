#include "dijkstra.h"

unsigned
dijkstra (graph_t *g, int from, int to, int *count, int **out)
{
    pqueue_t *q = pqueue_new(MIN_PRIORITY);
    unsigned *dist = calloc(g->count, sizeof(unsigned));
    int *done = calloc(g->count, sizeof(int));
    int *path = malloc(g->count * sizeof(int));
    memset(dist, INFINITY, g->count * sizeof(unsigned));
    memset(path, -1, g->count * sizeof(int));
    dist[from] = 0;

    enqueue(q, from, 0);

    int i, j;
    
    while (q->count)
    {
        item_t p = dequeue(q);
        if (done[p.value])
            continue;

        int *nodes = g->nodes[p.value];

        for (i = 0; i < g->count; i ++)
        {
            if (nodes[i] <= 0)
                continue;
            if (dist[i] > (dist[p.value] + nodes[i]))
            {
                dist[i] = nodes[i] + dist[p.value];
                enqueue(q, i, dist[i]);
                path[i] = p.value;
            }
        }

        done[p.value] = 1;
    }
 
    j = to;
    i = 1;
    
    done[0] = to;
    
    while (path[j] != -1)
        done[i ++] = j = path[j];

    done = realloc(done, i * sizeof(int));
    *count = i;
    int tmp;
    i --;
    while (j < i )
    {
        tmp = done[i];
        done[i --] = done[j];
        done[j ++] = tmp;
    }
    
    *out = done;
    unsigned r = dist[to];
    free(dist);
    free(path);
    pqueue_clear(q);
    return r;
}

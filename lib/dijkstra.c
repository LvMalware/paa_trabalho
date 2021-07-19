#include "dijkstra.h"

unsigned
dijkstra (graph_t *g, int from, int to, int *count, int **out)
{
    /* Dijkstra algorithm: find the minimal path between two nodes in a graph */
    
    /* creates a new pqueue_t structure (a priority queue) */
    pqueue_t *q = pqueue_new(MIN_PRIORITY);
    /* allocates an array which will hold the minimum path to each node from the
     * coming from the origin node.
     */
    unsigned *dist = malloc(g->count * sizeof(unsigned));
    /* the 'done' array will be used to check for already expanded nodes */
    int *done = calloc(g->count, sizeof(int));
    /* the 'parent' array will be used to recover the route that results in the
     * path with minimum cost.
     */
    int *parent = malloc(g->count * sizeof(int));
    /* when the algorithm starts, we don't know the distance to any nodes, so we
     * say it is infinity (in this case, just a really big value that will be
     * unlikely to be a real cost.
     */
    memset(dist, INFINITY, g->count * sizeof(unsigned));
    /* we also initialize the path with -1 so we can keep track of which nodes
     * have been updated to the minimum path
     */
    memset(parent, -1, g->count * sizeof(int));
    /* the distance from the origin node to itself is always 0 */
    dist[from] = 0;

    /* we start by enqueueing the origin node with a priority of 0 */
    enqueue(q, from, 0);

    /* NOTE: the priority of each node is equal to the cost to reach it from the
     * origin node.
     */
    
    int i, j;
    
    /* while the queue is not empty */
    while (q->count)
    {
        /* get the first item from the queue (which has the lowest cost and
         * therefore the maximum priority)
         */
        item_t p = dequeue(q);
        /* if this node was already expanded, we just ignore it */
        if (done[p.value])
            continue;

        /* get a pointer to the array of edges of the current node */
        int *nodes = g->nodes[p.value];
        /* loop through each node, updating the distances as needed */
        for (i = 0; i < g->count; i ++)
        {
            /* ensure we are only dealing with valid edges */
            if (nodes[i] <= 0)
                continue;
            /* if we can reach node i from the node being expanded with a cost
             * smaller than the previous known cost, we update the distance to
             * it and enqueue node i and its cost to be expanded later
             */
            if (dist[i] > (dist[p.value] + nodes[i]))
            {
                dist[i] = nodes[i] + dist[p.value];
                enqueue(q, i, dist[i]);
                /* we also update the parent to node i with p.value since it
                 * represents the node that has the lowest cost to reach it
                 * (for now)
                 */
                parent[i] = p.value;
            }
        }
        
        /* once we have expanded a node, mark it as done */
        done[p.value] = 1;
    }
 
    /* now that we have the cost of the minimum path, lets find out which nodes
     * belong to it.
     */
 
    /* j will be used to track the current node */
    j = to;
    /* i will be used to index the array with the path */
    i = 1;
    
    /* we are going to reuse the done array to recover the path, but just for
     * the sake of readability we set its address to a pointer named 'path'
     */
    int *path = done;
    /* the path always starts at the destination node (we will need to invert
     * the array later in order to get the right order)
     */
    path[0] = to;
    
    /* now we loop though the parent array until we find a node having -1 as its
     * parent which means it is the origin node (since we came from it, there is
     * no parent node) and add each node in the path to our array
     */
    while (parent[j] != -1)
        path[i ++] = j = parent[j];
    
    /* then we reallocate the path array to be the size of the real path */
    path = realloc(path, i * sizeof(int));
    /* set the the variable pointed by count to be the length of the path */
    *count = i;
    /* as the path was recovered in the reverse order (coming from the
     * destination to the origin), we will invert the elements in the array to
     * have them in the corrent order
     */

    /* tmp is just an auxiliary variable for swapping values */
    int tmp;
    /* i holds the size of the path, but we are gonna use it as an index */
    i --;
    j = 0;
    /* now we swap each element at position j with the element at positon i,
     * incrementing i and decrementing j at each iteration until they have the
     * same value, which indicates we already swapped everything.
     */
    while (j < i )
    {
        tmp = done[i];
        done[i --] = done[j];
        done[j ++] = tmp;
    }
    
    /* then we set the pointer pointed to by 'out' to point to the path address
     * (it seems that there are just too many p's in this sentence, right? )
     */
    *out = path;
    /* recover the minimum cost to the destination from the dist array */
    unsigned r = dist[to];
    /* free the dist and parent arrays, as well as the queue */
    free(dist);
    free(parent);
    pqueue_clear(q);
    /* return the total cost of the minimum path */
    return r;
}

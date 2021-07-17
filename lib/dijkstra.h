#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include <string.h>
#include <stdlib.h>

#include "graph.h"
#include "pqueue.h"

/* as far as we are concerned, this is infinity! */
#define INFINITY 0xFFFFFFFF

unsigned dijkstra (graph_t *, int, int, int *, int **);

#endif /* dijkstra.h */

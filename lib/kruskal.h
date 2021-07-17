#ifndef _KRUSKAL_H
#define _KRUSKAL_H 1

#include <string.h>
#include <stdlib.h>

#include "sort.h"
#include "graph.h"

typedef struct
{
    int orig;
    int dest;
    int cost;
} edge_t;

int kfind (int *, int);

void kunion (int *, int, int);

int kruskal_circular_check (int *, int, int);

graph_t *kruskal_mst (graph_t *);

#endif /* kruskal.c */
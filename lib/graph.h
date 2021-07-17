#ifndef _GRAPH_H
#define _GRAPH_H 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct graph
{
    int count;
    int edges;
    int total;
    int **nodes;
    char **names;
} graph_t;

graph_t *graph_new (int);

void graph_edge (graph_t *, int, int, int);

void graph_free (graph_t *);

void graph_show (graph_t *);

void graph_name_node (graph_t *, int, const char *); 

int graph_has_edge (graph_t *, int, int);

#endif /* graph.h */

#ifndef _PQUEUE_H
#define _PQUEUE_H 1

#ifndef VALUE_T
#define VALUE_T int
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct item
{
    unsigned priority;
    VALUE_T  value;
} item_t;

typedef enum
{
    MIN_PRIORITY = 0,
    MAX_PRIORITY = 1
} priority_t;

typedef struct pqueue
{
    int         count;
    item_t     *items;
    priority_t  prio;
} pqueue_t;

pqueue_t *pqueue_new (priority_t);

void pqueue_clear (pqueue_t *);

void enqueue (pqueue_t *, VALUE_T, unsigned);

item_t dequeue (pqueue_t *);

item_t pqueue_first (pqueue_t *);

#endif /* pqueue.h */

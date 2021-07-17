#include "pqueue.h"

pqueue_t *
pqueue_new (priority_t type)
{
    pqueue_t *pq = calloc(1, sizeof(pqueue_t));
    pq->prio = type;
    return pq;
}

void
pqueue_clear (pqueue_t *pq)
{
    if (!pq)
        return;
    free(pq->items);
    free(pq);
}

void
enqueue (pqueue_t *pq, VALUE_T v, unsigned p)
{
    int i, index = pq->count;
    if (!pq->items)
    {
        pq->items = calloc(1, sizeof(item_t));
        pq->items[0].value = v;
        pq->items[0].priority = p;
        pq->count = 1;
        return;
    }

    for (i = 0; i < pq->count; i ++)
        if (pq->items[i].priority > p)
            index = i;
    pq->items = realloc(pq->items, (pq->count + 1) * sizeof(item_t));

    if (index < pq->count)
        memmove(&pq->items[index + 1], &pq->items[index],
                (pq->count - index) * sizeof(item_t));
    pq->items[index].value = v;
    pq->items[index].priority = p;
    pq->count ++;
}

item_t
dequeue (pqueue_t *pq)
{
    int index = (pq->prio == MAX_PRIORITY) ? pq->count - 1 : 0;
    item_t v = pq->items[index];
    memmove(&pq->items[index], &pq->items[index + 1],
            (pq->count - index) * sizeof(item_t));
    pq->items = realloc(pq->items, (pq->count - 1) * sizeof(item_t));
    pq->count --;

    return v;
}

item_t
pqueue_first (pqueue_t *pq)
{
    int index = (pq->prio == MAX_PRIORITY) ? pq->count - 1 : 0;
    return pq->items[index];
}

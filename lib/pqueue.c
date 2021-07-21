#include "pqueue.h"

pqueue_t *
pqueue_new (priority_t type)
{
    /* creates a new pqueue_t structure */
    pqueue_t *pq = calloc(1, sizeof(pqueue_t));
    
    /* the priority queue may work in two different ways:
     * 1. as a minimum priority queue, where the lowest value has the highest
     *    priority and is therefore returned first. and
     * 2. as a maximum priority queue, where the highest value has the highest
     *    priority and is therefore returned first.
     * the .prio item of the pqueue_t structure controls which of these modes
     * is used by the queue.
     */
    pq->prio = type;
    return pq;
}

void
pqueue_clear (pqueue_t *pq)
{
    /* free the memory used by the items and the struct pointed to by pq */
    if (!pq)
        return;
    free(pq->items);
    free(pq);
}

void
enqueue (pqueue_t *pq, VALUE_T v, unsigned p)
{
    /* adds a new element with the given priority to the queue */
    
    /* NOTE: this implementation works as a dynamic vector where the elements
     * are stored in a sorted manner by their priority using an insertion sort
     */

    int index = pq->count;
    /* if the queue is empty, just allocate the first element and set its value
     * and priority, updating the count of the queue
     */    
    if (!pq->items)
    {
        pq->items = calloc(1, sizeof(item_t));
        pq->items[0].value = v;
        pq->items[0].priority = p;
        pq->count = 1;
        return;
    }
    
    /* if we reach this point, the queue already has elements and we will need
     * to search for the right position where to insert the new element
     */
    
    int l = 0, u = pq->count;
    /* do a binary search to find the correct index to place the new item */
    while (l < u)
    {
        /* index receives the element in the middle of the interval l-u */
        index = (l + u) / 2;
        /* if the item at INDEX has a bigger priority, set to u INDEX - 1 */
        if (pq->items[index].priority > p)
            u = index - 1;
        /* if the item at INDEX has a lower priority, set l to INDEX + 1 */
        else if (pq->items[index].priority < p)
            l = index + 1;
        /* if this item has the same priority, we break out of the loop */
        else
            break;
    }
    
    /* when we reach this point, we can just insert the element at INDEX */
    
    /* NOTE: if we never find an element with the same priority of our new item,
     * l will eventually be equal to u and the loop will be terminated. In this
     * case, the item before index has a smaller priority and the item after it
     * has a bigger one, so we must insert the new item in between them, which
     * is accomplished by doing a regular insertion at INDEX
     */

    /* once we known where to insert the new value, we reallocate the memory, so
     * we can fit one more item
     */
    pq->items = realloc(pq->items, (pq->count + 1) * sizeof(item_t));

    /* if we are inserting between two other elements, we need to make space for
     * the new item, so we use memmove to get the item at index out of the way,
     * moving it "to the right"
     */
    if (index < pq->count)
        memmove(&pq->items[index + 1], &pq->items[index],
                (pq->count - index) * sizeof(item_t));

    /* finally we can set the value and priority of the new item */
    pq->items[index].value = v;
    pq->items[index].priority = p;
    /* and then update the number of items on the queue */
    pq->count ++;
}

item_t
dequeue (pqueue_t *pq)
{
    /* removes and returns from the queue the item with highest priority */
    
    /* on a MAX_PRIORITY queue, we always get the last element as it will have
     * the highest priority value because of the way we sort the elements when
     * they are inserted. On a MIN_PRIORITY queue, it is the inverse, so we just
     * set index to 0
     */
    int index = (pq->prio == MAX_PRIORITY) ? pq->count - 1 : 0;

    /* recover the item_t structure at INDEX */
    item_t v = pq->items[index];
    
    /* moves the items "to the left" to fill the gap of the removed item */
    memmove(&pq->items[index], &pq->items[index + 1],
            (pq->count - index) * sizeof(item_t));
    /* NOTE: if we are removing the last element of the queue, nothing will
     * happen as a result of the previous code. Since pq->count - index is equal
     * to 1, we will be basically moving the last item to where it already is
     * so we can just leave it to memmove() to deal with that
     */
     
    /* reallocate the memory to free the space left by the removed item */
    pq->items = realloc(pq->items, (pq->count - 1) * sizeof(item_t));
    /* decrements the number of items on the queue */
    pq->count --;
    /* return the item_t structure that we saved before */
    return v;
}

item_t
pqueue_first (pqueue_t *pq)
{
    /* returns the first element on the queue without removing it */
    
    /* select the first element's index based on the type of priority used */
    int index = (pq->prio == MAX_PRIORITY) ? pq->count - 1 : 0;

    /* return the item_t structure at that position */
    return pq->items[index];
}

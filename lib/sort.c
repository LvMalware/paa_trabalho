#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"
#include "stack.h"

void
swap (void *array, size_t size, int i, int j)
{
    /* swap the contents of two memory areas of the given size,
     * indexed with i and j inside the array
     */

    /* allocate a memory area of the needed size */    
    void *temp = malloc(size);
    /* copy SIZE bytes from array[i] to the temp variable */
    memcpy(temp, array + i, size);
    /* copy SIZE bytes from array[j] to array[i] */
    memcpy(array + i, array + j, size);
    /* copy SIZE bytes from temp to array[j] */
    memcpy(array + j, temp, size);
    /* free the temp memory */
    free(temp);
    /* TODO: we could have temp as an static variable and only allocate the
     * memory once, without freeing it every time (but I'm not sure if it would
     * be a good idea)
     */
}

int
pivot (void *array, int l, int u, size_t size,
        int (*cmp) (const void *, const void *))
{
    /* finds the pivot of the subarray delimited by l and u */
    
    /* starts i as l and j as u (they will be updated later) */
    int i = l;
    int j = u;
    
    /* loop while the lower boundary i is smaller than the upper boundary u */
    while (i < j)
    {
        /* starting from the current j, we iterate from right to left until we
         * find the first element that is smaller than the one at position i,
         * swap these these values and break out of the for loop
         */
        for (; j > i; j -= size)
        {
            if (cmp(array + i, array + j) > 0)
            {
                swap(array, size, i, j);
                break;
            }
        }
        
        /* starting from the current i, we iterate from left to right until we
         * find the first element that is bigger than the one at position j,
         * swap these values and break out of the for loop
         */
        for (; i < j; i += size)
        {
            if (cmp(array + i, array + j) > 0)
            {
                swap(array, size, i, j);
                break;
            }
        }
        
    }
    
    /* NOTE: at the end of this process, each element of the subarray l-u at the
     * left of i is smaller than the value at position i and every value at the
     * right of is bigger than it and so we conclude the 'conquer' step
     */
     
    /* return the position of the pivot (currently at position i) */
    return i;
}

void
sort (void *array, size_t nmemb, size_t size,
       int (*cmp) (const void *, const void *))
{
    /* Interactive Quicksort: sorts a given array of nmemb elements of the given
     * size, using cmp to compare them (maybe that's overkill?)
     */
     
    /* we use two stacks to keep track of the lower and upper boundaries of the
     * subarrays of the main array
     */
    stack_t *upper = stack_new();
    stack_t *lower = stack_new();
    
    /* start by pushing 0 as the lower boundary */
    stack_push(lower, 0);
    /* and (nmemb - 1) * size as the upper boundary */
    stack_push(upper, (nmemb - 1) * size);
    
    /* NOTE: as we implemented this algorithm in a generic manner (trying to
     * mimic the qsort from stdlib.h), we need to know the real boundary where
     * the array ends, so we must multiply the index of the last element by the
     * size of each individual element
     */

    /* loop while both stacks have elements */
    while (!stack_empty(upper) && !stack_empty(lower))
    {
        /* pops the lower boundary of the subarray into l */
        int l = stack_pop(lower);
        /* pops the upper boundary of the subarray into u */
        int u = stack_pop(upper);

        /* if l >= u, there's nothing to be done */
        if (l >= u)
            continue;

        /* call pivot() to retrive the index of pivot (conquer) */
        int i = pivot(array, l, u, size, cmp);
        
        /* check the pivot value and updates the boundaries for the subarrays
         * of the current subarray (divide)
         */
        if (i == l)
        {
            /* if the pivot is at the start of the array, just advance one
             * potition in the array by adding the size of one element to i
             */
            stack_push(lower, i + size);
            /* keep the upper boundary */
            stack_push(upper, u);
        }
        else if (i == u)
        {

            /* keep the lower boundary */
            stack_push(lower, i);
            /* if the pivot is at the end of the array, subtract the size of one
             * element from it to exclude the last element
             */
            stack_push(upper, u - size);
        }
        else
        {
            /* if the pivot is somewhere else, we will divide the subarray into
             * two, excluding the pivot
             */
            
            /* keep the lower boundary */
            stack_push(lower, l);
            /* exclude the pivot by subtracting size from i */
            stack_push(upper, i - size);
            /* exclude the pivot by adding size to i */
            stack_push(lower, i + size);
            /* keep the upper boundary */
            stack_push(upper, u);
        }
        /* repeat */
    }
}

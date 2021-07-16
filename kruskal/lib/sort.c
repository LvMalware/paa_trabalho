#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"
#include "stack.h"

void
swap (void *array, size_t size, int i, int j)
{
    void *temp = malloc(size);
    memcpy(temp, array + i, size);
    memcpy(array + i, array + j, size);
    memcpy(array + j, temp, size);
    free(temp);
}

int
pivot (void *array, int l, int u, size_t size,
        int (*cmp) (const void *, const void *))
{
    int i = l;
    int j = u;
    
    while (i < j)
    {
        for (; j > i; j -= size)
        {
            if (cmp(array + i, array + j) > 0)
            {
                swap(array, size, i, j);
                break;
            }
        }
        
        for (; i < j; i += size)
        {
            if (cmp(array + i, array + j) > 0)
            {
                swap(array, size, i, j);
                break;
            }
        }
        
    }
    
    return i;
}

void
sort (void *array, size_t nmemb, size_t size,
       int (*cmp) (const void *, const void *))
{
    stack_t *upper = stack_new();
    stack_t *lower = stack_new();
    
    stack_push(lower, 0);
    stack_push(upper, (nmemb - 1) * size);
    while (!stack_empty(upper) && !stack_empty(lower))
    {
        int l = stack_pop(lower);
        int u = stack_pop(upper);

        if (l >= u)
            continue;

        int i = pivot(array, l, u, size, cmp);

        if (i == l)
        {
            stack_push(lower, i + size);
            stack_push(upper, u);
        }
        else if (i == u)
        {
            stack_push(lower, i);
            stack_push(upper, u - size);
        }
        else
        {
            stack_push(lower, l);
            stack_push(upper, i - size);
            /*  */
            stack_push(lower, i + size);
            stack_push(upper, u);
        }
    }
}

#include "stack.h"

stack_t *
stack_new ()
{
    /* allocates memory for a new stack_t structure */
    return (stack_t *) calloc(1, sizeof(stack_t));
}

void
stack_clear (stack_t *stack)
{
    /* clear the stack, freeing each item of it */
    
    item_t *n, *i = stack->top;
    while (i)
    {
        n = i->next;
        free(i);
        i = n;
    }
    free(stack);
}

void
stack_push (stack_t *stack, VTYPE val)
{
    /* pushes a new element to the stack */
    item_t *i = (item_t *) calloc(1, sizeof(item_t));
    i->value = val;
    i->next = stack->top;
    stack->top = i;
    stack->count ++;
}

VTYPE
stack_pop (stack_t *stack)
{
    /* pops (remove and return) the first item on the stack */
    item_t *i = stack->top;
    stack->top = i->next;
    stack->count --;
    VTYPE v = i->value;
    free(i);
    return v; 
}

int
stack_empty (stack_t *stack)
{
    /* check if the stack is empty */
    
    return stack->count == 0;     
}

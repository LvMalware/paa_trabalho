#include "stack.h"

stack_t *
stack_new ()
{
    return (stack_t *) calloc(1, sizeof(stack_t));
}

void
stack_clear (stack_t *stack)
{
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
    item_t *i = (item_t *) calloc(1, sizeof(item_t));
    i->value = val;
    i->next = stack->top;
    stack->top = i;
    stack->count ++;
}

VTYPE
stack_pop (stack_t *stack)
{
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
    return stack->count == 0;     
}
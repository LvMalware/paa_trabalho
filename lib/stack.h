#ifndef _STACK_H
#define _STACK_H 1

#include <stdlib.h>

#ifndef VTYPE
#define VTYPE int
#endif /* VTYPE */

typedef struct item {
    VTYPE value;
    struct item *next;
} item_t;

typedef struct stack
{
    int count;
    item_t *top;
} stack_t;


stack_t *stack_new ();

void stack_clear (stack_t *);

void stack_push (stack_t *, VTYPE val);

VTYPE stack_pop (stack_t *);

int stack_empty (stack_t *);

#endif /* stack.h */
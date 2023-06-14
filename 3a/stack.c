#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack *stack_create(const int capacity) {
    Stack *st = (Stack *) calloc(1, sizeof(Stack));
    st->mas = (int *) calloc(capacity, sizeof(int));
    return st;
}

int stack_push(Stack *st, int x) {
    st->mas[st->top++] = x;
    return 0;
}

int stack_pop(Stack *st, int *info) {
    *info = st->mas[--st->top];
    return 0;
}

int stack_empty(Stack *st) {
    return st->top == 0;
}

void stack_destroy(Stack *st) {
    free(st->mas);
    free(st);
}

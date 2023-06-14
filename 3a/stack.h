#ifndef LAB3_STACK_H
#define LAB3_STACK_H

typedef struct Stack {
    int *mas; // Массив элементов стэка
    int top;
} Stack;

Stack *stack_create(const int);

int stack_push(Stack *, int);

int stack_pop(Stack *, int *);

int stack_empty(Stack *);

void stack_destroy(Stack *);

#endif //LAB3_STACK_H

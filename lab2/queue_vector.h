#ifndef LAB2_QUEUE_VECTOR_H
#define LAB2_QUEUE_VECTOR_H

//#include "func.h"

struct Item;

typedef struct Queue {
    int head;
    int tail;
    int n;
    int capacity;
    struct Item *mas; //Указатель на начала массива Item
} Queue;

Queue *queue_new();

int queue_delete(Queue *);

int queue_front(Queue *, struct Item **);

int queue_push(Queue *, struct Item *);

int queue_pop(Queue *);

void queue_print(const Queue *);

void queue_deleteFull(Queue *, int );

#endif //LAB2_QUEUE_VECTOR_H

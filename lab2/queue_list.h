#ifndef LAB2_QUEUE_LIST_H
#define LAB2_QUEUE_LIST_H

typedef struct List {
    struct Item *data;
    struct List* next;
} List;

typedef struct Queue {
    List *head;
    List *tail;
    int n;
} Queue;

Queue *queue_new(int);

void queue_delete(Queue *queue, int);

int queue_push(Queue *queue, struct Item *);

int queue_pop(Queue *queue, struct Item *);

void queue_print(const Queue *queue);

int queue_front(Queue *q, struct Item **);

int queue_empty(Queue *q);

void queue_deleteFull(Queue *, int );

void queue_auto(Queue *, int);

#endif

#ifndef LAB2_QUEUE_LIST_H
#define LAB2_QUEUE_LIST_H

//typedef struct Item { //Создадим вектор Item'ов для каждой стойки
//    char id;
//    int ta;
//    int ts;
//}Item;

typedef struct List {
    struct Item *data;
    struct List* next;
} List;

typedef struct Queue {
    List *head;
    List *tail;
    int n;
    int capacity;
} Queue;

Queue *queue_new(int);

void queue_delete(Queue *queue, int);

int queue_push(Queue *queue, struct Item *);

int queue_pop(Queue *queue);

void queue_print(const Queue *queue);

int queue_front(Queue *q, struct Item **);

void queue_deleteFull(Queue *, int );


//void reading (char s[], int racks, Queue *, int *);
//
//void conclusion(Queue *q, int, int);
//
//int correction(Queue *, int *, int, int, int);

#endif

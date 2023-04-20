 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef queue_vector

#include "queue_vector.h"
#include "func.h"


Queue* queue_new(const int capacity) {
    Queue *q = (Queue *) calloc(1, sizeof(Queue));
    q->capacity = capacity;
    q->head = q->tail = q->n = 0;
    q->mas = (Item *) calloc(q->capacity, sizeof(Item));
    return q;
}

int queue_delete(Queue *queue) {
    free(queue->mas);
    free(queue);
}

int queue_push(Queue *queue, Item *item) {
    if (queue->n == queue->capacity) return 1; //Отказ
    queue->mas[queue->tail] = *item;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->n++;
    return 0;
}

int queue_pop(Queue *queue, Item *info) {
    if (queue->n == 0) return 1; //Отказ
    *info = queue->mas[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->n--;
    return 0;
}

void queue_print(const Queue *queue) {
    int i = queue->head;
    int j = 0;
    if (queue->n == 0) {
        printf("\n");
        return ;
    }
    while (j < queue->n) { //j < queue->n
        printf("%c \t", queue->mas[i].id);
        i = (i + 1) % queue->capacity;
        ++j;
    }
    printf("\n");
}

void queue_deleteFull(Queue *q, int racks) {
    for (int i = 0; i < racks; ++i) {
        free(q[i].mas);
    }
    free(q);
}

int queue_front(Queue *q, Item **new) {
    if (q->n == 0) return 1;
    *new = &q->mas[q->head];
    return 0;
}

int queue_empty(Queue* q) {
    if (q->n == 0) return 1;
    return 0;
}

#endif

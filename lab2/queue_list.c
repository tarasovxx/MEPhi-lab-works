#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue_list.h"
#include "func.h"

#define DELIM " "

#ifdef queue_list

Queue *queue_new(const int c) {
    Queue *q = (Queue *) calloc(1, sizeof(Queue));
    return q;
}

void queue_delete(Queue *queue, int racks) {
    for (int i = 0; i < racks; ++i) {
        List *ptr = queue[i].head, *ptr_prev;
        while (ptr) {
            ptr_prev = ptr;
            ptr = ptr->next;
            free(ptr_prev);
        }
    }
    free(queue);
}

void queue_deleteFull(Queue *q, int racks) {
    for (int i = 0; i < racks; ++i) {
        List *ptr = q[i].head, *ptr_prev;
        while (ptr) {
            ptr_prev = ptr;
            ptr = ptr->next;
            free(ptr_prev);
        }
    }
    free(q);
}

int queue_push(Queue *queue, Item *t) {
    List *new = malloc(1 * sizeof(List));
    new->data = t;
    new->next = NULL;
    if (queue->n == 0) { //!queue->head->data
        //queue->head->data = calloc(1, sizeof(Item));
        queue->head = new;
        queue->tail = new;
    }
    else {
        //queue->head->next = calloc(1, sizeof(List));
        queue->tail->next = new;
        queue->tail = new;
    }
    queue->n++;

    return 0;
}
//
//int queue_pop(Queue *queue, Item *info) {
//    if (queue->head == NULL)
//        return 1;
//
//    info = queue->head->data;
//
//    List *head = queue->head;
//    queue->head = head->next;
//    free(head);
//
//    if (queue->head == NULL)
//        queue->tail = NULL;
//
//    return 0;
//}

 int queue_pop(Queue *queue, Item *info) { //, char *output
     if (!queue->head) {
         return 1;
     }
     info = queue->head->data;
     if (queue->head == queue->tail) {
         queue->tail = NULL;
     }
     List *head = queue->head;
     queue->head = head->next;
     queue->n--;
     free(head);
     return 0;
 }

void queue_print(const Queue *queue) {
    List *ptr = queue->head; //if (queue->mas[i + 1].ta > m) break;
    int i = 0;
    if (queue->n == 0) {
        printf("\n");
        return ;
    }
    while (ptr && i < queue->n) {
        printf("%c\t", ptr->data->id);
        ptr = ptr->next;
        ++i;
    }
    printf("\n");
}

int queue_front(Queue *q, Item **new1) {
    if (q->n == 0) return 1;
    if (q->head && q->head->data) *new1 = q->head->data;
    return 0;
}

int queue_empty(Queue *q) {
    if (q->n == 0) return 1;
    return 0;
}




#endif

#ifndef LAB2_FUNC_H
#define LAB2_FUNC_H

#if defined (queue_vector)
#include "queue_vector.h"  //gcc ------- -D queue_list -o lab2
#elif defined (queue_list)
#include "queue_list.h"
#endif


typedef struct Item {
    char id;
    int ta;
    int ts;
} Item;

void reading (Item *, int , Queue *, int , int);

void input(char *s, Item **arr, int *len);

int getInt(int *n);


#endif //LAB2_FUNC_H

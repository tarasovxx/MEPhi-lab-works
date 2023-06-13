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

int min(int , int );

void input(char *, Item **, int *);

int getInt(int *);

int countMoment(Queue *, Item* , int , int , int ,  int *, int *, int *, int , int);

int checker(Queue *mas, int *tl, Item *new, int i, int racks, int *mintl);

int balance(Queue *mas, int *tl, Item *new1, Item *tmp, int i, int racks, int *ind, int *met, int moment, int *mintl, int len);

int print(Queue *mas, Item *tmp, int moment, int racks, int i);

int checkEmpty(Queue *, int);



#endif //LAB2_FUNC_H

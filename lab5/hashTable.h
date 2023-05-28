#ifndef LAB5_HASHTABLE_H
#define LAB5_HASHTABLE_H

#include "func.h"

typedef struct KeySpace {
    int key;
    int info;
    Pair *parent; // дополнительно опциональное поле
    struct KeySpace *next;
} KeySpace;

typedef struct Table {
    int msize;
    int csize;
    KeySpace **ks;
} Table;


// Вставка, поиск, удаление
int cantorPairing(Pair *);

int hash(int );

KeySpace *tableFind(Table *, Pair *);

int tableInsert(Table **, Pair *, int);

int tableInsertWithParent(Table **, Pair *, Pair *);

int tableDelete(Table **, Pair *);

void tableDestroy(Table ** );


#endif //LAB5_HASHTABLE_H

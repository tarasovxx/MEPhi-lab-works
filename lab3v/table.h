#ifndef LAB3_TABLE_H
#define LAB3_TABLE_H

//#include "func.h"

typedef struct Table {
    struct KeySpace *ks;
    unsigned int msize;
    unsigned int csize;
} Table;


int D_Add(Table *); // вставка элемента

int D_Find(Table *); // поиск элемента

int D_Delete(Table *); // удаление элемента

int D_Show(Table *); //Вывод

int D_CheckRelease(Table *);

int D_Import(Table *);

int insert(Table *, char* , int);

int findKey(Table *, const char *);

int delete(Table *, const char * , int i);

int reorganize(Table *);

int delTable(Table *);


#endif //LAB3_TABLE_H

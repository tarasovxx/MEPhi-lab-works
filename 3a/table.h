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

int D_Reorganization(Table *);

int D_Import(Table *);

int insert(Table *, int , int , char* );

int findKey(Table *, int );

int findParent(Table *, int );

int delete(Table *, int );

int delete2(Table *, int );

void reorganize(Table *);

int delTable(Table *);

struct KeySpace *findNextItem(Table *);

int findAllParent(Table *, int *,  int );


#endif //LAB3_TABLE_H

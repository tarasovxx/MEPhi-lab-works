#ifndef LAB3_TABLE_H
#define LAB3_TABLE_H

#include "func.h"

typedef struct Table {
    KeySpace *ks;
    unsigned int msize;
    unsigned int csize;
} Table;


int D_Add(Table *); // вставка элемента

int D_Find(Table *); // поиск элемента

int D_Delete(Table *); // удаление элемента

int D_Show(Table *); //Вывод

int D_Reorganization(Table *);

int D_Import(Table *);

int insert(Table *t, int k, int par, char* info);

int find(Table *t, int k, int f);

int delete(Table *t, int k, int i);

int reorganize(Table *t);

int delTable(Table *);

#endif //LAB3_TABLE_H

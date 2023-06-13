#ifndef LAB3_TABLE_H
#define LAB3_TABLE_H

//#include "func.h"

typedef struct Table {
    struct KeySpace *ks;
    unsigned int msize; //Размер таблицы
    unsigned int csize; //Текущий размер таблицы
    FILE *fd; //дескриптор
} Table;




int D_Add(Table *); // вставка элемента

int D_Find(Table *); // поиск элемента

int D_Delete(Table *); // удаление элемента

int D_Show(Table *); //Вывод

int D_Reorganization(Table *);

//int D_Import(Table *);

int insert(Table *, int , int , char* );

//int findKey(Table *, int );

int findParent(const Table *, int );

int delete(Table *, int , int );

int delete2(Table *, int , int );

int reorganize(Table *);

int delTable(Table *);

char *findInfo(const Table *, int );

int find(const Table *, int );

int D_Load(Table *);

int save(Table *t);

#endif //LAB3_TABLE_H

#ifndef LAB3G_USERFUNC_H
#define LAB3G_USERFUNC_H

#include "hashTable.h"

int D_Add(Table *), // вставка элемента

D_Find(Table *), // поиск элемента

D_Delete(Table *), // удаление элемента

D_Show(Table *), //Вывод

D_CheckRelease(Table *), // реорганизация таблицы (сборщик мусора)

D_Import(Table *);

int dialog(const char *msgs[], int N);

int D_Load(Table *);

#endif //LAB3G_USERFUNC_H

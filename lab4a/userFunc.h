#ifndef LAB4A_USERFUNC_H
#define LAB4A_USERFUNC_H

#include <stdlib.h>

#include "binTree.h"


// функции для организации диалога;
// при обнаружении конца файла возвращают 0
int dialog(const char *msgs[], int);	// выбор номера альтернативы

int D_Add(Node **),	// вставка элемента в таблицу

D_Find(Node **),	// поиск элемента в таблице

D_Delete(Node **),	// удаление элемента из таблицы

D_Show(Node **);	// вывод содержимого таблицы



#endif //LAB4A_USERFUNC_H

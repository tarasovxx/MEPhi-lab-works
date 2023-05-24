#ifndef LAB4A_USERFUNC_H
#define LAB4A_USERFUNC_H

#include <stdlib.h>

#include "binTree.h"
//BST - binary search tree

// функции для организации диалога;
// при обнаружении конца файла возвращают 0
int dialog(const char *msgs[], int);	// выбор номера альтернативы

int D_Add(Node **),	// вставка элемента в BST

D_Find(Node **),	// поиск элемента в BST

D_Delete(Node **),	// удаление элемента из BST

D_Show(Node **),	// вывод содержимого BST

D_Traversal(Node **),    // Прямой обход в дереве с прошивкой в заданном диапазоне

D_FindSpecial(Node **),   // специльный поиск элемента в BST

D_Import(Node **), // Импорт из файла

D_Timing(Node **); // Таймирование


#endif //LAB4A_USERFUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binTree.h"

// Функция включения в дерево нового элемента.
// В таблицу включается копия передаваемой информации
int insert(Node **proot, char *k, int info) {
    Node *ptr = *proot, *par = NULL;
    while (ptr) {
        par = ptr;
//        if (strcmp(ptr->key, k) == 0)
//            return 0; // Дублирование возможно
        if (strcmp(k, ptr->key) < 0)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }
    ptr = (Node *) calloc(1, sizeof(Node));
    ptr->key = strdup(k);
    ptr->info = info;
    ptr->parent = par;
    if (par == NULL)
        *proot = ptr;
    else
        if (strcmp(k, par->key) < 0)
            par->left = ptr;
        else
            par->right = ptr;
    return 1;
}

// Функция удаления из таблицы элемента, заданного ключом.
// Необходимо освободить память, занятую и элементом списка,
// и информацией
int delNode(Node **proot, char *key) {
    Node *y = NULL, *par = NULL, *ptr = NULL, *x = findOne(*proot, key);
    if (x == NULL)
        return 0;
    /////////////////////
    if (x->left == NULL || x->right == NULL) // Хотя бы один сын пуст
        y = x;
    else
        y = findMin(x->right); // Если оба сына - не нулевые
    /////////////////////
    if (y->left)
        ptr = y->left;
    else
        ptr = y->right;
    par = y->parent;
    /////////////////////
    if (ptr)
        ptr->parent = par;
    /////////////////////
    if (!par)
        *proot = ptr;

    /////////////////////
    if (par->left == y)
        par->left = ptr;
    else
        par->right = ptr;
    /////////////////////
    if (y != x)
        x->key = y->key;
    free(y->key);
    free(y);
    return 1;
}

// Функция прямого обхода дерева
void directTreeTraversal(Node *root) {
    if (!root) return ;
    else {
        printf("%s ", root->key);
        directTreeTraversal(root->left);
        directTreeTraversal(root->right);
    }
}

// Функция поиска в таблице элемента, заданного ключом
int findAll(Node **masPtr, Node *ptr, char *k) {
    int i = 0;
    while (ptr){
        if (strcmp(ptr->key, k) == 0) {
            masPtr[i] = ptr;
            ++i;
            //masPtr = realloc(masPtr, i * sizeof(Node *));
        }
        ptr = (strcmp(k, ptr->key) < 0) ? ptr->left : ptr->right;
    }
    return i == 0 ? -1 : i;
}

Node *findOne(Node *ptr, char *k) {
    while (ptr){
        if (strcmp(ptr->key, k) == 0) {
            return ptr;
        }
        ptr = (strcmp(k, ptr->key) < 0) ? ptr->left : ptr->right;
    }
    return NULL;
}

// Функция поиска в дереве минимального элемента
Node *findMin(Node *ptr) {
    if (!ptr)
        return NULL;
    while (ptr->left)
        ptr = ptr->left;
    return ptr;
}

// Функция вывода содержимого таблицы
// Прямой обход
void putTree(Node *ptr, int level) {
    int i = level;
    //printf("%s\n", ptr->key);
    if (ptr) {
        putTree(ptr->left, level + 1);
        while (i-- > 0)
            printf("  ");
        printf("%s\n", ptr->key);
        putTree(ptr->right, level + 1);
    }
}

// Функция освобождения памяти.
void delTree(Node **proot)
{
    if (*proot){
        delTree(&((*proot)->left));
        delTree(&((*proot)->right));
        free((*proot)->key);
        free(*proot);
    }
    *proot = NULL;
}

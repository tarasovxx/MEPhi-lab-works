#ifndef LAB4A_BINTREE_H
#define LAB4A_BINTREE_H

#include "otherFunc.h"

// Tree node
typedef struct Node{
    Item *key1;
    Item *key2;
    struct Node *left,			// left subtree
                *middle,        // middle subree
                *right,		    // right subtree
                *temp,
                *parent;		// parent node // optional
    unsigned int info;
} Node;

// функции для работы с деревом
Node *insert(Node *, char *, int );	// вставка элемента в дерево

int insert_to_node(Node *, char *, int, int); // Вствка эоемента в узел

Node *delete(Node *, char *, int);	// удаление элемента из дерева

int remove_from_node(Node *, char *m, int);  // Удавление элемента из узла

struct Node *find(Node *, char *, int);  // Поиск только одного элеимента (в случае дублирования самого старшего)

void delTree(Node **);		// освобождение памяти

void treeTraversal(Node *, char *, char *); //Прямой обход дерева в заданном диапазоне ключей

Node* split(Node *, char * , int , int);

int is_leaf(Node *root);

void swap(char **, char **);

void sort2(char **, char **);

void sort3(char **, char **, char **);

void sort(Node *);

void print_tree(Node *, int );

Node *getMin(Node *);

Node *getMax(Node *);

Node *merge(Node *);

Node *redistribute(Node *);

Node *fix(Node *);

int checkLastRelease(Node *, char *, int *);

void psevdoDel(Node *);

#endif //LAB4A_BINTREE_H

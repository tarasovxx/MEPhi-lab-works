#ifndef LAB4A_BINTREE_H
#define LAB4A_BINTREE_H

// Tree node
typedef struct Node{
    char *key;
    struct Node *left,			// left subtree
                *right,		    // right subtree
                *parent;		// parent node
    unsigned int info;
} Node;


// функции для работы с деревом
int insert(struct Node **, char *, int);	// вставка элемента

int delNode(struct Node **, char *);	// удаление элемента

int findAll(struct Node **, struct Node *, char *);	// поиск элемента по ключу

struct Node *findOne(struct Node *, char *);

struct Node *findMin(struct Node *);		// поиск минимального элемента

void putTree(struct Node *, int);	// вывод дерева

void delTree(struct Node **);		// освобождение памяти

void directTreeTraversal(Node * ); //Прямой обход дерева


#endif //LAB4A_BINTREE_H

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
int insert(Node **, char *, int);	// вставка элемента

Node *deleteNode(Node *, char *);	// удаление элемента

int findAll(Node **, Node *, char *);	// поиск элемента по ключу

struct Node *findOne(Node *, char *);  // Поиск только одного элеимента (в случае дублирования самого старшего)

struct Node *findMin(Node *);		// поиск минимального элемента

void putTree(Node *, int);	// вывод дерева

void delTree(Node **);		// освобождение памяти

void directTreeTraversalInTheRange(Node *, char *, char *); //Прямой обход дерева в заданном диапазоне ключей

//int lowerBound(Node **, Node * , char *); // Функция специального поиска

int lowerBound(Node **masPtr, Node* root, char* key, int currDist, int dist, int* i);

int countingСoincidences(char *str1, char *str2);



#endif //LAB4A_BINTREE_H

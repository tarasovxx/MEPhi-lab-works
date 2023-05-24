#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binTree.h"
#include "error.h"
#include "checkFunc.h"


// Функция включения в дерево нового элемента.
// В таблицу включается копия передаваемой информации
int insert(Node **proot, char *k, int info) {
    Node *ptr = *proot, *par = NULL;
    while (ptr && ptr->key) {
        par = ptr;
        if (strcmp(ptr->key, k) >= 0) {
            ptr = ptr->left;
        }
        else {
            ptr = ptr->right;
        }
//        if (strcmp(ptr->key, k) == 0)
//            return 0; // Дублирование возможно
//        int m = strcmp(k, ptr->key);
//        if (strlen(k) < strlen(par->key)) {
//            ptr = ptr->left;
//        }
//        else if (strlen(k) > strlen(par->key)) {
//            ptr = ptr->right;
//        }
//        else {
//            if (strcmp(k, par->key) <= 0)
//                ptr = ptr->left;
//            else
//                ptr = ptr->right;
//        }
    }
    ptr = (Node *) calloc(1, sizeof(Node));
    ptr->key = strdup(k);
    ptr->info = info;
    ptr->parent = par;
    if (par == NULL)
        *proot = ptr;
    else
//        if (strlen(k) < strlen(par->key)) {
//            par->left = ptr;
//        }
//        else if (strlen(k) > strlen(par->key)) {
//            par->right = ptr;
//        }
//        else {
        if (strcmp(par->key, k) >= 0)
            par->left = ptr;
        else
            par->right = ptr;
        //}
    return OK;
}

// Функция удаление элемента из дерева
// При дублированиий ключей удаляем самый страый
Node* deleteNode(Node* root, char* key) {
    if (root == NULL) {
        return root;
    }
    if (strcmp(key, root->key) < 0) {
        root->left = deleteNode(root->left, key);
    } 
    else if (strcmp(key, root->key) > 0) {
        root->right = deleteNode(root->right, key);
    } 
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root->key);
            free(root);
            root = NULL;
            return temp;
        } 
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root->key);
            free(root);
            root = NULL;
            return temp;
        }
        Node* temp = findMin(root->right);
        free(root->key);
        root->key = strdup(temp->key);
        root->info = temp->info;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

// Функция прямого обхода дерева
// Вывод сожержимого дерева в прямом порядке следования ключей в заданном диапазоне
void directTreeTraversalInTheRange(Node *root, char *a, char *b) {
    if (!root) return ;
    addThreadInOrder(root);
    Node *ptr = root;
    while (root) {
        if (strcmp(root->key, a) >= 0 && strcmp(root->key, b) <= 0) {
            printf("%s ", root->key);
            printf(" -> ");
        }
        if (root->left) {
            root = root->left;
            continue;
        }
        else if (root->right) {
            root = root->right;
            continue;
        }
        if (root->thread) {
            root = root->thread;
            continue;
        }
        root = root->left;
    }
//    else {
//        if (strcmp(root->key, a) >= 0 && strcmp(root->key, b) <= 0) printf("%s ", root->key);
//        directTreeTraversalInTheRange(root->left, a, b);
//        directTreeTraversalInTheRange(root->right, a, b);
//    }

}


// Функция поиска в таблице элемента, заданного ключом
int findAll(Node **masPtr, Node *ptr, char *k) {
    int i = 0;
    while (ptr){
        if (strcmp(ptr->key, k) == 0) {
            masPtr[i++] = ptr;
            //masPtr = realloc(masPtr, i * sizeof(Node *));
        }
        ptr = (strcmp(k, ptr->key) <= 0) ? ptr->left : ptr->right;
    }
    return i == 0 ? -1 : i;
}

Node *findOne(Node *ptr, char *k) {
    while (ptr){
        if (strcmp(ptr->key, k) == 0) {
            return ptr;
        }
        ptr = ((strcmp(k, ptr->key) < 0) || (strlen(k) < strlen(ptr->key))) ? ptr->left : ptr->right;
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

Node *findMax(Node *ptr) {
    if (!ptr) {
        return NULL;
    }
    while (ptr->right) {
        ptr = ptr->right;
    }
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
        if (ptr) printf("(%s;%d)\n", ptr->key, ptr->info);
        putTree(ptr->right, level + 1);
    }
}

// Функция освобождения памяти.
void delTree(Node **proot) {
    if (*proot){
        delTree(&((*proot)->left));
        delTree(&((*proot)->right));
        free((*proot)->key);
        free(*proot);
    }
    *proot = NULL;
}

//int lowerBound(Node **masPtr, Node *ptr, char *k) {
//    int i = 0;
//    int mini = -1;
//    while (ptr){
//        if (strcmp(ptr->key, k) == 0) {
//            masPtr[i] = ptr;
//            ++i;
//            //masPtr = realloc(masPtr, i * sizeof(Node *));
//        }
//        ptr = (strcmp(k, ptr->key) < 0) ? ptr->left : ptr->right;
//    }
//    return i == 0 ? -1 : i;
//}

int countingСoincidences(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int i = 0;
    while (str1[i] == str2[i]) ++i;
//    for (int i = 0; i < len1 && i < len2; ++i) {
//        if (str1[i] == str2[i]) dist++;
//    }
    return i;
}


// Функция для поиска элемента, наиболее близкого по значению к заданному,
// но не совпадающего с ним
//int lowerBound(Node **masPtr, Node* root, char* key) {
//    if (root == NULL) {
//        return 0;
//    }
//    int i = 0;
//    Node* closest = root;
//    Node *temp = NULL;
//    int dist = countingСoincidences(key, root->key);
//    // Потом мы наверняка отсеим точное совпадение поэтому не о чем переживать
//    Node* curr = root;
//    //if (strcmp(curr->key, key) != 0) masPtr[i++] = curr;
//    while (curr != NULL) {
//        // Если находим точное совпадение по ключу
////        if (strcmp(curr->key, key) == 0) {
////            int dist2, dist3;
////            if (curr->left != NULL) {
////                // Ищем наибольший элемент в левом поддереве
////                closest = findMax(curr->left);
////                dist2 = countingСoincidences(key, curr->key);
////                //closest = curr->left;
////                //while (closest->right != NULL) {
////                //    closest = closest->right;
////                //}
////            }
////            if (curr->right != NULL) {
////                // Ищем наименьший элемент в правом поддереве
////                temp = findMin(curr->right);
//////                if (closest == NULL) {
//////                    closest = curr->right;
//////                }
////                int dist3 = countingСoincidences(key, temp->key);
////
//////                if (strcmp(key, closest->key) - strcmp(key, curr->right->key) > 0) {
//////                    closest = curr->right;
//////                }
//////                while (closest->left != NULL) {
//////                    closest = closest->left;
//////                }
////            }
////            i = 0;
////            if (dist2 > dist3) masPtr[i++] = closest;
////            else if (dist2 < dist3) masPtr[i++] = temp;
////            else {
////                masPtr[i++] = closest;
////                masPtr[i++] = temp;
////            }
////            break;
////        }
//        // Если не находим точное совпадение по ключу, сравниваем значения ключей
//        // Если currDist >= dist, то мы нашли более близкое совпадение, чем было
//        int currDist = 0;
//        if (strcmp(curr->key, key) != 0) currDist = countingСoincidences(key, curr->key);
//        if (currDist > dist) {
//            i = 0;
//            masPtr[i++] = curr;
//            dist = currDist;
//        }
//        else if (currDist == dist && dist != 0) masPtr[i++] = curr;
////        if (closest == NULL || strcmp(key, closest->key) - strcmp(key, curr->key) > 0) {
////            closest = curr;
////        }
//        lowerBound(masPtr, root->left, key);
//        lowerBound(masPtr, root->right, key);
////        if (strcmp(key, curr->key) < 0) {
////            curr = curr->left;
////        }
////        else {
////            curr = curr->right;
////        }
//    }
//    if (i == 0) masPtr[i++] = root;
//    return i;
//}



int lowerBound(Node **masPtr, Node* root, char* key) {
    if (root == NULL) {
        return 0;
    }
    addThreadInOrder(root);
    int flag = 0, currDist = 0, dist = 0, len = 0;
    while (root) {
        flag = strcmp(root->key, key);
        if (flag != 0) currDist = countingСoincidences(key, root->key);
        if (currDist > dist && flag != 0) {
            masPtr[len++] =root;
            dist = currDist;
        }
        else if (currDist == dist && dist != 0 && flag != 0) masPtr[len++] = root;
        if (root->left) {
            root = root->left;
            continue;
        }
        else if (root->right) {
            root = root->right;
            continue;
        }
        if (root->thread) {
            root = root->thread;
            continue;
        }
        root = root->left;
    }
    //lowerBound(masPtr, root->left, key, currDist, dist, i);
    //lowerBound(masPtr, root->right, key, currDist, dist, i);
    if (dist == 0) return 0;
    return len;
}

void addThreadInOrder(Node* root) {
    if (root == NULL)
        return;
    if (root->left && root->right && !root->left->left && !root->right->right) {
        root->left->thread = root->right;
    }
    // Если есть левое поддерево, устанавливаем нить для самого правого узла в левом поддереве
    if (root->left != NULL) {
        Node* rightMost = root->left;
        while (rightMost->right != NULL || rightMost->left != NULL)
            if (rightMost->right) rightMost = rightMost->right;
            else rightMost = rightMost->left;
//        if (rightMost->right == NULL) {
//        }
        if (!rightMost->left && !rightMost->right && !rightMost->thread) rightMost->thread = root->right;
    }
//    if (root->right != NULL) {
//        Node* leftRightMost = root;
//        while (leftRightMost->left != NULL)
//            leftRightMost = leftRightMost->left;
//
//        if (!leftRightMost->left && !leftRightMost->right) leftRightMost->thread = root->right;
//    }
    // Рекурсивно вызываем функцию для левого поддерева
    addThreadInOrder(root->left);
    // Рекурсивно вызываем функцию для правого поддерева
    addThreadInOrder(root->right);
}


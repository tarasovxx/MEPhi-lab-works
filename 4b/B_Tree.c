#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "B_Tree.h"
#include "error.h"
#include "checkFunc.h"
#include "otherFunc.h"

int insert_to_node(Node *root, char *k, int value, int order) { // Вставка ключа K в вершину, не в дерево!
    //if (!root) root = calloc(1, sizeof(Node));
    if (root->key1 && root->key2) {
        return 1;
    }
    if (root->key1 == NULL) {
        root->key1 = calloc(1, sizeof(Item));
        root->key1->data = strdup(k);
        root->key1->order = order;
        root->info = value;
    }
    else if (root->key2 == NULL) {
        root->key2 = calloc(1, sizeof(Item));
        root->key2->data = strdup(k);
        root->key2->order = order;
        root->info = value;
    }
    sort(root);
    return 0;

}
Node *insert(Node *root, char *key, int value) { // Всегда возвращаем корень дерева, так как он может меняться
    if (!root) { // Если дерево пусто
        Node *newNode = calloc(1, sizeof(Node));
        newNode->key1 = calloc(1, sizeof(Item));
        newNode->key1->data = strdup(key);
        newNode->info = value;
        return newNode;
    }
    int checker = 0, order = 0;
    //Если root - это лист
    if (is_leaf(root)) {
        Node *glav = root;
        while (glav->parent) glav = glav->parent;
        checkLastRelease(glav, key, &order);
        checker = insert_to_node(root, key, value, order);
    }
    else if (strcmp(key, root->key1->data) <= 0) {
        insert(root->left, key, value);
    }
    else if ((root->key1 && !root->key2) || (root->key2 && strcmp(key, root->key2->data) <= 0)) {
        if (!root->middle) root->middle = insert(root->middle, key, value);
        else insert(root->middle, key, value);
    }
    else {
        insert(root->right, key, value);
    }
    return checker == 1 ? split(root, key, order, 0) : root;
}

Node *split(Node *root, char *k, int ord, int flag) {
    //Создаём две ноды, которые имеют такого же родителя как и рахделяющийся элемент
    int ch = 0;
    Node *x = calloc(1, sizeof(Node));
    x->key1 = calloc(1, sizeof(Item));
    Node *y = calloc(1, sizeof(Node));
    y->key1 = calloc(1, sizeof(Item));
    char *k_dup = strdup(k);
    if (flag) free(k);
    sort3(&root->key1->data, &root->key2->data, &k_dup);
    //if (strcmp(root->key1, k) > 0) swap(&root->key1, &k);
    x->key1->data = root->key1->data;
    x->key1->order = root->key1->order;
    x->info = root->info;
    y->key1->data = k_dup;
//    int ord = 0;
//    Node *glav = root;
//    while (glav->parent) glav = glav->parent;
//    checkLastRelease(glav, k, &ord);
    y->key1->order = ord;
    y->info = root->info;
    x->parent = root->parent;
    y->parent = root->parent;
    x->left = root->left;
    x->middle = root->middle;
    y->left = root->right;
    y->middle = root->temp;
    if (x->left)  x->left->parent = x;    // Правильно устанавливаем "родителя" "сыновей".
    if (x->middle) x->middle->parent = x;   // После разделения, "родителем" "сыновей" является "дедушка",
    if (y->left) {
        if (strcmp(y->left->key1->data, y->key1->data) <= 0) y->left->parent = y;    // Поэтому нужно правильно установить указатели.
        else swap(&y->left->key1->data, &y->key1->data);
    }
    if (y->middle) y->middle->parent = y;
    if (root->parent) {
        ch = insert_to_node(root->parent, root->key2->data, root->info, root->key2->order);
        if (ch != 1) {
            free(root->key2->data);
            free(root->key2);
            root->key2 = NULL;
        }
//        if (ch == 1) {
//            //free(root->key2->data);
//            if (root->key2 && root->key2->data) {
//                free(root->key2->data);
//                free(root->key2);
//                root->key2 = NULL;
//            }
//            x->parent = root->parent;
//            y->parent = root->parent;
//        }
        if (root->parent->left == root) root->parent->left = NULL;
        else if (root->parent->middle == root) root->parent->middle = NULL;
        else if (root->parent->right == root) root->parent->right = NULL;

        // Дальше происходит своеобразная сортировка ключей при разделении.
        if (root->parent->left == NULL) {
            root->parent->temp = root->parent->right;
            root->parent->right = root->parent->middle;
            root->parent->middle = y;
            root->parent->left = x;
        } else if (root->parent->middle == NULL) {
            root->parent->temp = root->parent->right;
            root->parent->right = y;
            root->parent->middle = x;
        } else {
            root->parent->temp = y;
            root->parent->right = x;
        }
        Node *tmp = root->parent;
	char *newKey = NULL;
        int targetInfo = 0;
        if (ch == 1)  {
            targetInfo = root->key2->order;
            newKey = strdup(root->key2->data);
            free(root->key2->data);
            free(root->key2);
            root->key2 = NULL;
        }
        free(root->key1);
        free(root);
        root = NULL;

        //psevdoDel(root);
        //root = tmp;
        //free(root->key1->data);
        return ch != 1 ? tmp : split(tmp, newKey, targetInfo, 1);	//free(root);
    }
    else {
        x->parent = root;   // Так как в эту ветку попадает только корень,
        y->parent = root;   // то мы "родителем" новых вершин делаем разделяющийся элемент.
        // Преобразовать в 2 вершину
        //free(root->key1->data);
        root->key1->data = root->key2->data;
        root->key1->order = root->key2->order;
        free(root->key2);
        root->key2 = NULL;

        root->left = x;
        root->middle = y;
        root->right = NULL;
        return ch != 1 ? root : split(root->parent, root->key2->data, root->key2->order, 0);;
    }
}

// Функция удаления элемента по ключу
Node *delete(Node *root, char *key, int order) {
    Node *target = find(root, key, order);

    if (!target) return NULL;
    Node *mini = NULL;
    if (strcmp(target->key1->data, key) == 0) mini = getMin(target->middle);
    if (!mini) mini = getMin(target->right);

    if (mini) { // Свопаем ключи
        int information = mini->info;
        if (target->key2 && strcmp(target->key2->data, key) == 0) {
            swap(&mini->key1->data, &target->key2->data);
            int ch = mini->key1->order;
            mini->key1->order = target->key1->order;
            target->key1->order = ch;
            //target->key2->data = tmp;
            mini->info = target->info;
            target->info = information;
            target = mini;
        }
        else {
            swap(&mini->key1->data, &target->key1->data);
            int ch = mini->key1->order;
            mini->key1->order = target->key1->order;
            target->key1->order = ch;
            //mini->key1 = target->key1;
            //target->key1->data = tmp;
            mini->info = target->info;
            target->info = information;
            target = mini;
        }
    }
    remove_from_node(target, key, order);

    return fix(target);  //вызываем функцию для восстановления свойст дерева
}

Node *fix(Node *leaf) {
    if (!leaf->key1 && !leaf->key2 && !leaf->parent) { // Случай 0, когда удаляем единственный ключ в дереве
        //free(leaf->key1->data);
        //if (leaf->key2->data) free(leaf->key2->data);
        free(leaf);
        leaf = NULL;
        return NULL;
    }
    if (leaf->key1 || leaf->key2) { // Случай 1, когда вершина, в которой удалили ключ, имела два ключа
        if (leaf->parent) return fix(leaf->parent);
        else return leaf;
    }

    Node *parent = leaf->parent;
    if ((parent->left && parent->left->key1 && parent->left->key2) || (parent->middle && parent->middle->key1 && parent->middle->key2) || (parent->key1 && parent->key2)) leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
    else if ((parent->key1 && parent->key2) && (parent->right->key1 && parent->right->key2)) leaf = redistribute(leaf); // Аналогично
    else leaf = merge(leaf); // Случай 3, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину

    return fix(leaf);
}

Node *redistribute(Node *leaf) {
    Node *parent = leaf->parent;
    Node *first = parent->left;
    Node *second = parent->middle;
    Node *third = parent->right;

    if ((parent->key1 && parent->key2) && ((first->key1 && !first->key2) || (!first->key1 && first->key2)) && ((second->key1 && !second->key2) || (!second->key1 && second->key2)) && ((third->key1 && !third->key2) || (!third->key1 && third->key2))) {
        if (first == leaf) {
            parent->left = parent->middle;
            parent->middle = parent->right;
            parent->right = NULL;
            insert_to_node(parent->left, parent->key1->data, parent->info, parent->key1->order);
            parent->left->right = parent->left->middle;
            parent->left->middle = parent->left->left;

            if (leaf->left != NULL) parent->left->left = leaf->left;
            else if (leaf->middle != NULL) parent->left->left = leaf->middle;

            if (parent->left->left != NULL) parent->left->left->parent = parent->left;

            remove_from_node(parent, parent->key1->data, parent->key1->order);
            free(first);
        } else if (second == leaf) {
            insert_to_node(first, parent->key1->data, parent->info, parent->key1->order);
            remove_from_node(parent, parent->key1->data, parent->key1->order);
            if (leaf->left != NULL) first->right = leaf->left;
            else if (leaf->middle != NULL) first->right = leaf->middle;

            if (first->right != NULL) first->right->parent = first;

            parent->middle = parent->right;
            parent->right = NULL;

            free(second);
        } else if (third == leaf) {
            insert_to_node(second, parent->key2->data, parent->key2->order, parent->key2->order);
            parent->right = NULL;
            remove_from_node(parent, parent->key2->data, parent->key2->order);
            if (leaf->left != NULL) second->right = leaf->left;
            else if (leaf->middle != NULL) second->right = leaf->middle;

            if (second->right != NULL)  second->right->parent = second;

            free(third);
        }
    } else if ((parent->key1 && parent->key2) && ((first->key1 && first->key2) || (second->key1 && second->key2) || (third->key1 && third->key2))) {
        if (third == leaf) {
            if (leaf->left != NULL) {
                leaf->middle = leaf->left;
                leaf->left = NULL;
            }

            insert_to_node(leaf, parent->key1->data, parent->info, parent->key1->order);
            if (second->key1 && second->key2) {
                parent->key2 = second->key2;
                remove_from_node(second, second->key2->data, second->key2->order);
                leaf->left = second->right;
                second->right = NULL;
                if (leaf->left != NULL) leaf->left->parent = leaf;
            } else if (first->key1 && first->key2) {
                parent->key2 = second->key1;
                leaf->left = second->middle;
                second->middle = second->left;
                if (leaf->left != NULL) leaf->left->parent = leaf;

                second->key1 = parent->key1;
                parent->key1 = first->key2;
                remove_from_node(first, first->key2->data, first->key2->order);
                second->left = first->right;
                if (second->left != NULL) second->left->parent = second;
                first->right = NULL;
            }
        } else if (second == leaf) {
            if (third->key1 && third->key2) {
                if (leaf->left == NULL) {
                    leaf->left = leaf->middle;
                    leaf->middle = NULL;
                }
                insert_to_node(second, parent->key2->data, parent->info, parent->key2->order);
                parent->key2 = third->key1;
                remove_from_node(third, third->key1->data, third->key1->order);
                second->middle = third->left;
                if (second->middle != NULL) second->middle->parent = second;
                third->left = third->middle;
                third->middle = third->right;
                third->right = NULL;
            } else if (first->key1 && first->key2) {
                if (leaf->middle == NULL) {
                    leaf->middle = leaf->left;
                    leaf->left = NULL;
                }
                insert_to_node(parent, parent->key1->data, parent->info, parent->key1->order);
                parent->key1 = first->key2;
                remove_from_node(first, first->key2->data, first->key2->order);
                second->left = first->right;
                if (second->left != NULL) second->left->parent = second;
                first->right = NULL;
            }
        } else if (first == leaf) {
            if (leaf->left == NULL) {
                leaf->left = leaf->middle;
                leaf->middle = NULL;
            }
            insert_to_node(first, parent->key1->data, parent->info, parent->key1->order);
            if (second->key1 && second->key2) {
                parent->key1 = second->key1;
                remove_from_node(second, second->key1->data, second->key1->order);
                first->middle = second->left;
                if (first->middle != NULL) first->middle->parent = first;
                second->left = second->middle;
                second->middle = second->right;
                second->right = NULL;
            } else if (third->key1 && third->key2) {
                parent->key1 = second->key1;
                second->key1 = parent->key2;
                parent->key2 = third->key1;
                remove_from_node(third, third->key1->data, third->key1->order);
                first->middle = second->left;
                if (first->middle != NULL) first->middle->parent = first;
                second->left = second->middle;
                second->middle = third->left;
                if (second->middle != NULL) second->middle->parent = second;
                third->left = third->middle;
                third->middle = third->right;
                third->right = NULL;
            }
        }
    } else if ((parent->key1 || parent->key2)) {
        insert_to_node(leaf, parent->key1->data, parent->info, parent->key1->order);

        if (first == leaf && (second->key1 && second->key2)) {
            parent->key1 = second->key1;
            remove_from_node(second, second->key1->data, second->key1->order);

            if (leaf->left == NULL) leaf->left = leaf->middle;

            leaf->middle = second->left;
            second->left = second->middle;
            second->middle = second->right;
            second->right = NULL;
            if (leaf->middle != NULL) leaf->middle->parent = leaf;
        } else if (second == leaf && (first->key1 && first->key2)) {
            parent->key1 = first->key2;
            remove_from_node(first, first->key1->data, first->key1->order);

            if (leaf->middle == NULL) leaf->middle = leaf->left;

            leaf->left = first->right;
            first->right = NULL;
            if (leaf->left != NULL) leaf->left->parent = leaf;
        }
    }
    return parent;
}

Node *merge(Node *leaf) {
    Node *parent = leaf->parent;

    if (parent->left == leaf) {
        //if (!parent->middle) parent->middle = calloc(1, sizeof(Node));
        insert_to_node(parent->middle, parent->key1->data, parent->info, parent->key1->order);
        parent->middle->right = parent->middle->middle;
        parent->middle->middle = parent->middle->left;

        if (leaf->left != NULL) parent->middle->left = leaf->left;
        else if (leaf->middle != NULL) parent->middle->left = leaf->middle;

        if (parent->middle->left != NULL) parent->middle->left->parent = parent->middle;

        remove_from_node(parent, parent->key1->data, parent->key1->order);
        free(parent->left);
        parent->left = NULL;
    } else if (parent->middle == leaf) {
        insert_to_node(parent->left, parent->key1->data, parent->info, parent->key1->order);

        if (leaf->left != NULL) parent->left->right = leaf->left;
        else if (leaf->middle != NULL) parent->left->right = leaf->middle;

        if (parent->left->right != NULL) parent->left->right->parent = parent->left;

        remove_from_node(parent, parent->key1->data, parent->key1->order);
        free(parent->middle);
        parent->middle = NULL;
    }

    if (parent->parent == NULL) {
        Node *tmp = NULL;
        if (parent->left != NULL) tmp = parent->left;
        else tmp = parent->middle;
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}

int remove_from_node(Node *root, char *k, int order) {
    if (root->key1 && strcmp(root->key1->data, k) == 0 && order == root->key1->order) {
        free(root->key1->data);
        free(root->key1);
        root->key1 = root->key2;
        root->key2 = NULL;
    }
    else if (root->key2 && strcmp(root->key2->data, k) == 0 && root->key2->order == order) {
        free(root->key2->data);
        free(root->key2);
        root->key2 = NULL;
    }
    return 0;
}

Node *getMin(Node *root) {
    if (!root) return root;
    if (!(root->left)) return root;
    else return getMin(root->left);
}

Node *getMax(Node *root) {
    if (!root) return root;
    if (!(root->right) && !(root->middle) && !(root->left)) return root;
    if (!(root->right) && !(root->middle)) {
        if (strcmp(root->left->key1->data, root->key1->data) >= 0) {
            return root->left;
        }
    }
    if (!root->right) return getMax(root->middle);
    else return getMax(root->right);
}

// Функция вывода дерева "В виде дерева" (Повернутое на 90)
void print_tree(Node *root, int depth) {
    if (root == NULL) {
        return;
    }
    print_tree(root->right, depth + 1);
    print_tree(root->middle, depth + 1);
    printf("%*s(", depth * 4, "");
    if (root->key2 != NULL) {
        printf("%s;%u|", root->key1->data, root->info);
        printf("%s;%u", root->key2->data, root->info);
    } else {
        printf("%s;%u", root->key1->data, root->info);
    }
    printf(")\n");
    print_tree(root->left, depth + 1);
}

Node *find(Node *root, char *k, int order) {
    if (!root) return NULL;
    if (root->key1 && strcmp(root->key1->data, k) == 0 && root->key1->order == order) return root;
    if (root->key2 && strcmp(root->key2->data, k) == 0 && root->key2->order == order) return root;
    Node* result = find(root->left, k, order);
    if (result) return result;

    result = find(root->middle, k, order);
    if (result) return result;

    return find(root->right, k, order);
}

// Вывод в прямом порядке следования ключей, не входящих в заданный диапазон [a, b]
void treeTraversal(Node *root, char *a, char *b) {
    if (!root) return ;
    //if (strcmp(root->key1->data, a) < 0 || strcmp(root->key1->data, b) > 0) printf("%s: %d\n", root->key1->data, root->info);
    //if (root->key2 && (strcmp(root->key2->data, a) < 0 || strcmp(root->key2->data, b) > 0)) printf("%s: %d1\n", root->key2->data, root->info);
    treeTraversal(root->left, a, b);
    treeTraversal(root->middle, a, b);
    treeTraversal(root->right, a, b);
}

// Функция освобождения памяти.
void delTree(Node **proot) {
    if (*proot){
        delTree(&((*proot)->left));
        delTree(&((*proot)->middle));
        delTree(&((*proot)->right));
        if ((*proot)->key1) {
            free((*proot)->key1->data);
            free((*proot)->key1);
        }
        if ((*proot)->key2) {
            free((*proot)->key2->data);
            free((*proot)->key2);
        }
        free(*proot);
    }
    *proot = NULL;
}

void psevdoDel(Node *root) {
    if (root) {
        psevdoDel(root->left);
        psevdoDel(root->middle);
        psevdoDel(root->right);
        if (root->key1) {
            free(root->key1);
        }
        if (root->key2) {
            free(root->key2);
        }
        free(root);
    }

}

// Проверка узла, является ли он листом
int is_leaf(Node *root) {
    if (!root->left && !root->middle && !root->right) return 1;
    return 0;
}

// Функция перемены
void swap(char **x, char **y) {
    char *temp = *x;
    *x = *y;
    *y = temp;

}

// Сравнение двух строк
void sort2(char **x, char **y) {
    if (strcmp(*x, *y) > 0) {
        swap(x, y);
    }
}

void sort3(char **x, char **y, char **z) {
    if (strcmp(*x, *y) > 0) swap(x, y);
    if (strcmp(*x, *z) > 0) swap(x, z);
    if (strcmp(*y, *z) > 0) swap(y, z);
}

// Сортировка при вставке в узел
void sort(Node *root) {
    if (!root->key1 && !root->key2) return;
    if (root->key1 && root->key2) sort2(&root->key1->data, &root->key2->data);
}

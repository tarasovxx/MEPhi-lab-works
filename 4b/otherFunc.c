#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "otherFunc.h"
#include "B_Tree.h"

int checkLastRelease(Node *root, char *key, int *version) {
    // Проход по всему дереву
    if (!root) return (*version);
    if (root->key1 && strcmp(root->key1->data, key) == 0) (*version)++;
    if (root->key2 && strcmp(root->key2->data, key) == 0) (*version)++;
    checkLastRelease(root->left, key, version);
    checkLastRelease(root->middle, key, version);
    checkLastRelease(root->right, key, version);
}

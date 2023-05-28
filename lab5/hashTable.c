#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "hashTable.h"

int cantorPairing(Pair *key) {
    return ((key->x + key->y) * (key->x + key->y + 1) / 2 + key->y);
}

int hash(int l) {
    int hash = INT_MAX;
    for (int i = 0; i < sizeof(l); ++i) {
        hash = 37 * hash + ((l >> 8 * i) & 0xff);
    }
    return abs(hash);
}

KeySpace *tableFind(Table *t, Pair *key) {
    int unionKey = cantorPairing(key);
    int j = hash(unionKey) % t->msize;
    KeySpace *p = t->ks[j];
    while (p) {
        if (unionKey == p->key)
            return p;
        p = p->next;
    }
//    for (KeySpace *p = t->ks[j]; p->next != NULL; p = p->next) {
//        if (unionKey == p->key)
//            return p;
//    }
    return NULL;
}

int tableInsertWithParent(Table **t, Pair *key, Pair *value) {
    KeySpace *sign = tableFind(*t, key);
    int unionKey = cantorPairing(key);
    int j = hash(unionKey) % (*t)->msize;
    if (sign != NULL) { //То есть уже есть такой
        sign = (*t)->ks[j];
        KeySpace *ptr = sign;
        while (sign  && sign->key != unionKey) {
            sign = sign->next;
        }
        if (sign) {
            sign->parent = value;
            sign->info = 1;
        }
        (*t)->ks[j] = ptr;
        return 1;
    }
    return 1;
}

int tableInsert(Table **t, Pair *key, int value) {
    KeySpace *sign = tableFind(*t, key);
    int unionKey = cantorPairing(key);
    int j = hash(unionKey) % (*t)->msize;
    if (sign != NULL) { //То есть уже есть такой
        sign = (*t)->ks[j];
        KeySpace *ptr = sign;
        while (sign  && sign->key != unionKey) {
            sign = sign->next;
        }
        if (sign) sign->info = value;
        (*t)->ks[j] = ptr;
        return 1;
    }
    KeySpace *newKs = calloc(1, sizeof(KeySpace));
    newKs->key = unionKey;
    newKs->info = value;
    newKs->next = (*t)->ks[j];
    (*t)->ks[j] = newKs;
    (*t)->csize++;
    return 1;
}

int tableDelete(Table **t, Pair *key) {
    int unionKey = cantorPairing(key);
    int j = hash(unionKey) % (*t)->msize;
    KeySpace *ptr = (*t)->ks[j], *prevPtr;
    while (ptr && ptr->key != unionKey) {
        prevPtr = ptr;
        ptr = ptr->next;
    }
    if (ptr == (*t)->ks[j]) {
        (*t)->ks[j] = (*t)->ks[j]->next;
    }
    else {
        prevPtr->next = ptr->next;
    }
    free(ptr);
}

void tableDestroy(Table **t) {
    for (size_t i = 0; i < (*t)->msize; ++i) {
        if ((*t)->ks) {
            KeySpace *ptr = (*t)->ks[i], *prevPtr = NULL;
            while (ptr) {
                prevPtr = ptr;
                ptr = ptr->next;
                free(prevPtr);
            }
        }
    }
    free((*t)->ks);
    free(*t);
}

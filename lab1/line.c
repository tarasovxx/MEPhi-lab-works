#include <stdio.h>
#include <stdlib.h>

#include "line.h"
#include "matrix.h"

int indI(line *ptr) {
    int ind_i = -1;
    for (int j = 0; j < ptr->size - 1; ++j) {
        if (ptr->data[j + 1] > ptr->data[j]) {
            ind_i = j + 1;
            break;
        }
    }
    return ind_i;
}

int indJ(line *ptr) {
    int ind_j = -1;
    for (int j = ptr->size - 1; j > 1; --j) {
        if (ptr->data[j - 1] > ptr->data[j]) {
            ind_j = j;
            break;
        }
    }
    return ind_j;
}

void swapInd(line *ptr, int i, int j) {
    if (i != -1 && j != -1) {
        int tmp = ptr->data[i];
        ptr->data[i] = ptr->data[j];
        ptr->data[j] = tmp;
    }
}
//
//int task(matrix *mn) {
//
//    for (int i = 0; i < mn->size; ++i) {
//        line ptr = mn->data[i];
//        int ind_i = -1, ind_j = -1;
//        for (int j = 0; j < ptr.size - 1; ++j) {
//            if (ptr.data[j + 1] > ptr.data[j]) {
//                ind_i = j + 1;
//                break;
//            }
//        }
//        for (int j = ptr.size - 1; j > 1; --j) {
//            if (ptr.data[j - 1] > ptr.data[j]) {
//                ind_j = j;
//                break;
//            }
//        }
//        if (ind_i != -1 && ind_j != -1) {
//            int tmp = ptr.data[ind_i];
//            ptr.data[ind_i] = ptr.data[ind_j];
//            ptr.data[ind_j] = tmp;
//        }
//    }
//}

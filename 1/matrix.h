#ifndef FUNC_H
#define FUNC_H

#include "line.h"

typedef struct matrix {
    int size; //Количество строк матрицы
    line *data;
}matrix;

int getInt(int *);
int input(matrix *);
void output(const char *msg, matrix );
void erase(matrix *);
int task(matrix *);


#endif

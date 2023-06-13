#ifndef LINE_H
#define LINE_H

typedef struct line {
    int size; //Длина строки
    int *data;
}line;

int indI(line *ptr);
int indJ(line *ptr);
void swapInd(line *ptr, int i, int j);

#endif

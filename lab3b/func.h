#ifndef LAB3_FUNC_H
#define LAB3_FUNC_H

typedef struct Item {
    char *info;
}Item;

typedef struct KeySpace {
    int busy;
    unsigned int key;
    unsigned int par;
    //Item *data;
    int offset; // Смещение в файле
    int len; //длина информации
} KeySpace;


int getInt(int *n);

char *getStr(const char *ms);

int dialog(const char *msgs[], int N);

char *getFileStr(FILE *);

int to_int(char *s);


#endif //LAB3_FUNC_H

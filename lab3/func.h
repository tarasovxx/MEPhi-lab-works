#ifndef LAB3_FUNC_H
#define LAB3_FUNC_H

typedef struct Item {
    //unsigned int key; //Спросить
    char *info;
    //int ind;
}Item;

typedef struct KeySpace {
    int busy;
    unsigned int key;
    unsigned int par;
    Item *data;
} KeySpace;

int getInt(int *n);

char *getStr(const char *ms);

int dialog(const char *msgs[], int N);

char *getFileStr(FILE *);

int to_int(char *s);


#endif //LAB3_FUNC_H

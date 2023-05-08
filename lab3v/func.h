#ifndef LAB3_FUNC_H
#define LAB3_FUNC_H

typedef struct Item {
    unsigned int x;
}Item;

typedef struct KeySpace {
    int busy;
    char * key;
    unsigned int release;
    Item * data;
} KeySpace;


int getInt(int *n);

char *getStr(const char *ms);

int dialog(const char *msgs[], int N);

char *getFileStr(FILE *);

int to_int(char *s);

int primeNumber(int n);



#endif //LAB3_FUNC_H

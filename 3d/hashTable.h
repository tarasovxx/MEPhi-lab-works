#ifndef LAB3G_HASHTABLE_H
#define LAB3G_HASHTABLE_H

typedef struct Item {
    char *key;
    unsigned int x;
}Item;

typedef struct Pair {
    char *str;
    unsigned int lastRelease;
} Pair;

typedef struct KeySpace {
    int busy;
    //char * key;
    unsigned int release;
    //Item * data;
    int offset; // Смещение в файле
    int len; // Длина информации ключ + информация

} KeySpace;

typedef struct Table {
    struct KeySpace *ks;
    unsigned int msize;
    unsigned int csize;
    int csizeRel;
    Pair *rel; // Массив структур элементов, который хранит последнюю версию элемента
    FILE *fd; // Файловый дескриптор
} Table;



int insert(Table *, char* , int);

int findKey(Table *, const char *);

int delete(Table *, const char * , int );

int reorganize(Table *);

int delTable(Table *);

int findRelease(Table *, const char *, int);

int binarySearch(const Table *, const char *);

int insertLastRelease(Table *, char *);

int save(Table *);

int create(Table *, char *, int );

int load(Table *, char *);

KeySpace *findNextItem(Table* );



#endif //LAB3G_HASHTABLE_H

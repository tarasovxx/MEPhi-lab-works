#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
//#include "error.h"
#include "hash.h"
#include "checkFunc.h"

#define SEP " "

int insertLastRelease(Table *t, char *key) {
    int ind = binarySearch(t, key);
    if (ind >= 0) {
        t->rel[ind].lastRelease++;
        return ind;
    }
    int i = t->csizeRel - 1;
    while (i >= 0 && (strcmp(t->rel[i].str, key) > 0)) {
        t->rel[i + 1].str = t->rel[i].str;
        t->rel[i + 1].lastRelease = t->rel[i].lastRelease;
        --i;
    }


    t->rel[i + 1].str = strdup(key);
    t->rel[i + 1].lastRelease = 0;
    t->csizeRel++;
    return i + 1;
}

int binarySearch(const Table * t, const char *key) {
    int left = 0, right = t->csizeRel - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (strcmp(t->rel[mid].str, key) == 0) {
            //t->ks->rel[mid].lastRelease++;
            return left;
        }
        if (strcmp(t->rel[mid].str, key) > 0) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1;
}

int findKey(Table *t, const char *k) {
    if (t->csize < 1) return -1;
    int j = hash(k) % t->msize;
    int n = 0;
    while (t->ks[j].busy  && n < t->msize) {
        fseek(t->fd, t->ks[j].offset, SEEK_SET);
        char buf[1024];
        fgets(buf, t->ks[j].len, t->fd);
        char *tok = strtok(buf, SEP);
        char *key = tok;
        if (hash(k)== hash(key)) return j;
        int step =  primeNumber(t->msize);  //По алгоритму Эратосфена находим ближайшее к msize простое число//t->msize % 2 == 0 ? 3 : 2;
        j = (j + step) % t->msize;
        n++;
    }
    return -1; //-1 Не нашли
}

KeySpace *findNextItem(Table* table) {
    static int currentIndex = -1;  // Статическая переменная для хранения текущего индекса

    while (currentIndex < table->msize - 1) {
        currentIndex++;
        KeySpace* ks = &(table->ks[currentIndex]);
        if (ks->busy) {
            // Элемент найден
            fseek(table->fd, table->ks[currentIndex].offset, SEEK_SET);
            return ks;
        }
    }

    // Все элементы просмотрены, сбрасываем текущий индекс
    currentIndex = -1;
    return NULL;
}


int findRelease(Table *t, const char *k, int rel) {
    if (t->csize < 1) return -1;
    int j = hash(k) % t->msize;
    int n = 0;
    int step =  primeNumber(t->msize);  //По алгоритму Эратосфена находим ближайшее к msize простое число//t->msize % 2 == 0 ? 3 : 2;
    while (n < t->msize) {
        //printf("%d\n\n\n\n", t->ks[j].release);
        fseek(t->fd, t->ks[j].offset, SEEK_SET);
        char buf[1024];
        fgets(buf, t->ks[j].len, t->fd);
        char *tok = strtok(buf, SEP);
        char *key = tok;
        if (t->ks[j].len > 0 && hash(k) == hash(key) && rel == t->ks[j].release) return j;
        j = (j + step) % t->msize;
        n++;
    }
    return -1; //-1 Не нашли
}


int insert(Table *t, char* k, int info) {
    if (t->csize >= t->msize) return 2; //таблица заполнена
    int j = (hash(k) % t->msize);
    int ind = insertLastRelease(t, k);
    int rel = t->rel[ind].lastRelease;
    int n = 0; //Кол-во просмотренных элементов
    int step = primeNumber(t->msize); //t->msize % 2 == 0 ? 3 : 2;
    while (t->ks[j].busy == 1 && n < t->msize) { //Ищем свободное место
        //if (t->ks[j].busy == 1 && strcmp(t->ks[j].key, k) == 0) rel++;
        j = (j + step) % t->msize;
        n++;
    } //Вставляем элемент на свободное место
    if (n < t->msize) {
        //t->ks[j].key = strdup(k);
        char str[12];
        sprintf(str, "%u", info);
        t->ks[j].len = strlen(k) + 1 + strlen(str) + 1 + 1;
        fseek(t->fd, 0, SEEK_END);
        t->ks[j].offset = ftell(t->fd);
        t->ks[j].busy = 1;
        t->ks[j].release = rel;
        char tempBuf[1024];
        snprintf(tempBuf, t->ks[j].len, "%s %s", k, str);
        fwrite(tempBuf, sizeof(char), t->ks[j].len - 1, t->fd);
        //fwrite(info, sizeof(unsigned int), 1, t->fd);
        //t->ks[j].data->x = info;
        t->csize++;
    }
    return 0; //Good
}


int delTable(Table *t) {
    for (int i = 0; i < t->msize; ++i) {
        //if (t->ks[i].data->info) {
//        if (t->ks[i].busy) {
//            free(t->ks[i].key);
//        }
        if (t->rel[i].str) free(t->rel[i].str);
        //free(t->ks[i].data->info);
        //free(t->ks[i].data);

        //free(&t->ks[i]);
        //}
    }
    //free(t->ks->data);
    free(t->ks);
    free(t->rel);
    free(t);
}
int delete(Table *t, const char *k, int i) {
    if (i < t->msize && i > -1)
        t->ks[i].busy = 0;
    //free(t->ks[i].data);
    //free(t->ks[i].key);
    //t->ks[i].key = NULL;
    t->csize--;
    return 0;
}

int load(Table *t, char *fname) { //Загрузка таблицы из существующего файла!
//Открываем существующий файл таблицы на чтение и запись
    //fopen_s(&(t->fd), fname, "r+b");
    //fopen_s(&(t->fd), fname, "r+b");
    t->fd = (FILE *) fopen(fname, "r+b");
    if (t->fd == NULL)
        return 0;
// Файл открыт, можно читать; Считываем размер вектора
    fread(&t->msize, sizeof(int), 1, t->fd);
    t->ks = (KeySpace *)calloc(t->msize, sizeof(KeySpace));
    t->rel = (Pair *) calloc(t->msize, sizeof(Pair));
    fread(&t->csize, sizeof(int), 1, t->fd);
    fread(t->ks, sizeof(KeySpace), t->msize, t->fd);
    return 1;
}

int create(Table *t, char *fname, int sz) {
// Должны создать пустую таблицу размером csize
// новый файл и зарезервировать в нём место для таблицы
    t->msize = sz;
    t->csize = 0;
    t->fd = fopen(fname, "w+b");
    if (/*fopen_s(&(t->fd), fname, "w+b")*/t->fd == NULL) {
        t->ks = NULL;
        return 0;
    }
    t->ks = (KeySpace *)calloc(t->msize, sizeof(KeySpace));
    t->rel = (Pair *) calloc(t->msize, sizeof(Pair));
    // Записываем в начало файла размер
    fwrite(&t->msize, sizeof(int), 1, t->fd);
    // Размер таблицы
    fwrite(&t->csize, sizeof(int), 1, t->fd);
    // Саму таблицу
    fwrite(t->ks, sizeof(KeySpace), t->msize, t->fd);
    return 1;
}

int save(Table *t) {
    fseek(t->fd, sizeof(int), SEEK_SET);
    fwrite(&t->csize, sizeof(int), 1, t->fd);
    fwrite(t->ks, sizeof(KeySpace), t->msize, t->fd);
    fclose(t->fd);
    t->fd = NULL;
    return 1;
}

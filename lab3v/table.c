#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h" //UNION
#include "error.h"
#include "hash.h"

int D_Add(Table *t) {
    int info, rc, n;
    puts("Enter key: -->");
    scanf("%*c");
    char *key = getStr("");
    if(key == NULL) return 0;
    puts("Enter info: -->");
    n = getInt(&info);
    if(n == 0) return 0;
    rc = insert(t, key, info);
    printf(errMsgs[rc]);
    free(key);
    return 1;
}

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
        if (hash(k)== hash(t->ks[j].key)) return j;
        int step =  primeNumber(t->msize);  //По алгоритму Эратосфена находим ближайшее к msize простое число//t->msize % 2 == 0 ? 3 : 2;
        j = (j + step) % t->msize;
        n++;
    }
    return -1; //-1 Не нашли
}

int findRelease(Table *t, const char *k, int rel) {
    if (t->csize < 1) return -1;
    int j = hash(k) % t->msize;
    int n = 0;
    int step =  primeNumber(t->msize);  //По алгоритму Эратосфена находим ближайшее к msize простое число//t->msize % 2 == 0 ? 3 : 2;
    while (n < t->msize) {
        //printf("%d\n\n\n\n", t->ks[j].release);
        if (t->ks[j].key && hash(k) == hash(t->ks[j].key) && rel == t->ks[j].release) return j;
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
        t->ks[j].key = strdup(k);
        t->ks[j].busy = 1;
        t->ks[j].release = rel;
        t->ks[j].data->x = info;
        t->csize++;
    }
    return OK; //Good
}

int D_Delete(Table *t) {
    int n, version;
    puts("Enter key, which you want to delete -->");
    scanf("%*c");
    char *k = getStr("");
    if (k == NULL) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    puts("Enter relese for this key, which you want to delete -->");
    n = getInt(&version);
    if (n == 0) return 0;
    int i = findRelease(t, k, version);
    if (i >= 0)
        delete(t, k, i);
    if (i < 0) {
        puts("This key was not found\n");
        return -1; //Такого ключа нет
    }
//    int i = find(t, k);
//    if (i < 0) return -1; //Такого ключа нет
//    t->ks[i].busy = 0;
    printf("The item table[%s] = %d was successfully deleted\n", k, t->ks[i].data->x);
    printf("You have deleted %d version of the element\n", t->ks[i].release);
    free(k);
    return 1; //Good
}

int D_Find(Table *t) {
    int  n;
    puts("Enter the key you want to find -->");
    scanf("%*c");
    char *k = getStr("");
    if(k == NULL) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    int i = findKey(t, k);
    if (i < 0 || t->ks[i].busy == 0){
        printf("Element not found\n");
        return -1; //Не найден
    }
    printf("Good! Element found! table[%s] = %d \n", k, t->ks[i].data->x); //Всё гуд, найден
    free(k);
}

int D_Show(Table *t) {
    puts("busy ||| key    |||  release ||| info");
    for (int i = 0; i < t->msize; ++i) {
        if (t->ks[i].key && t->ks[i].busy)
            printf("%d \t %s \t\t %d \t %d\n", t->ks[i].busy, t->ks[i].key, t->ks[i].release, t->ks[i].data->x);
        else
            printf("%d \t %s \t\t %d \t %d\n", t->ks[i].busy, t->ks[i].key, t->ks[i].release, t->ks[i].data->x);
    }
    return 1;
}

int D_CheckRelease(Table *t) {
    puts("Please select: \n1 - Find all versions of the item; \n2 - Find a single one;");
    int checker,userSelect;
    checker = getInt(&userSelect);
    if (checker == 0) return 0;
    Table *newTable = (Table *) calloc(1, sizeof(Table));
    int i = 0, sizeNewTable = 10;
    newTable->ks = calloc(sizeNewTable, sizeof(KeySpace));
    newTable->rel = calloc(sizeNewTable, sizeof(Pair));
    puts("Enter key: -->");
    scanf("%*c");
    char *key = getStr("");
    if (key == NULL) return 0;
    if (userSelect == 1) {
        int j = hash(key) % t->msize;
        int n = 0;
        while (t->ks[j].busy == 1 && n < t->msize) {
            int step = primeNumber(t->msize);//t->msize % 2 == 0 ? 3 : 2;
            if (t->ks[j].busy == 1 && hash(key) == hash(t->ks[j].key)) {
                //newTable->ks[i++] = t->ks[j];
                newTable->ks[i].busy = t->ks[j].busy;
                newTable->ks[i].key = strdup(t->ks[j].key);
                newTable->ks[i].release = t->ks[j].release;
                newTable->ks[i].data = malloc(sizeof(Item));
                newTable->ks[i++].data->x = t->ks[j].data->x;
                newTable->csize++;
                newTable->msize++;
                if (newTable->csize >= sizeNewTable) {
                    sizeNewTable *= 2;
                    newTable->ks = (KeySpace *) realloc(newTable->ks, sizeof(KeySpace) * sizeNewTable);
                }
            }
            j = (j + step) % t->msize;
            n++;
        }
    }
    else if (userSelect == 2) {
        puts("Enter the version of the item you want to find --->");
        int rel;
        checker = getInt(&rel);
        if (checker == 0) return 0;
        int j = findRelease(t, key, rel);
        if (j >= 0) {
            newTable->ks[i].busy = t->ks[j].busy;
            newTable->ks[i].key = strdup(t->ks[j].key);
            newTable->ks[i].release = t->ks[j].release;
            newTable->ks[i].data = malloc(sizeof(Item));
            newTable->ks[i++].data->x = t->ks[j].data->x;
            newTable->csize++;
            newTable->msize++;
        }


    }
    puts("****************************************************");
    D_Show(newTable);
    //free(newTable->ks);
    //free(newTable);
    delTable(newTable);
    free(key);
    return 1;

}

int D_Import(Table *t) {
    FILE *in;
    int n, c, i = 0, flag = 0;
    scanf("%*c");
    char *path = getStr("Enter the file name: ");
    //scanf("%s", path);
    in = fopen(path, "r"); //C:\labs_in_c\2semestr\lab3\input.txt
    if (in == NULL) {
        puts("Error, this file not found\n");
        return -3;
    }
    fscanf(in, "%d", &n);
    fscanf(in, "%*c");
    char s[80] = {};

    t->msize = n;
    t->ks = (KeySpace *) realloc(t->ks, (n + 1) * sizeof(KeySpace));
    int b, k, rel;
    while(!feof(in)) {
        fscanf(in, "%d %[^\n]s %*c %*c %d %*c %*c %d", &b, s, &rel, k);
        t->ks[i].busy = b;
        if (findKey(t, s) >= 0) {
            flag = 1;
            break;
        }
        t->ks[i].key = s;
        t->ks[i].release = rel;
        t->ks[i].data = calloc(1, sizeof(Item));
        t->ks[i].data->x = k;
        //int len = strlen(s);
//        for (int d = 0; d < len; ++d)
//            t->ks[i].data->info[d] = s[d];
        t->csize = i;
        if (flag == 0) ++i;
        flag = 0;

    }
    free(path);
    fclose(in);
    return 1;
}

int to_int(char *s) {
    int i = strlen(s), j = 0, ans = 0;
    while (j != i) {
        ans = (ans * 10) + (s[j] - '0');
        ++j;
    }
    return ans;
}

int delTable(Table *t) {
    for (int i = 0; i < t->msize; ++i) {
        //if (t->ks[i].data->info) {
        if (t->ks[i].busy) {
            free(t->ks[i].key);
        }
        if (t->rel[i].str) free(t->rel[i].str);
        //free(t->ks[i].data->info);
        free(t->ks[i].data);

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
    free(t->ks[i].key);
    t->ks[i].key = NULL;
    t->csize--;
    return OK;
}

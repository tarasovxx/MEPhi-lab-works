//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h"
#include "error.h"
#include <readline/readline.h>


int D_Add(Table *t) {
    int k, rc, n, par;
    puts("Enter key: -->");
    n = getInt(&k);
    if (n == 0) return 0; //Обнаружен конец файла
    puts("Enter par (If you don't want to specify enter 0): --->");
    n = getInt(&par);
    if (n == 0) return 0;
    while ((par != 0) && (find(t, par) < 0)) {
        if (par == k) break;
        puts("There is no such key in the table yet, please enter a new one, or 0\n");
        n = getInt(&par);
        if (n == 0) return 0;
    }
    scanf("%*c");
    char *info = readline("Enter info: ");
    if(info == NULL) return 0;
    rc = insert(t, k, par, info);
    free(info); //если элемент вставляется в таблицу - вставляется его копия
    printf(errMsgs[rc]);
    return 1;
}



int insert(Table *t, int k, int par, char* info) {
    int i = find(t, k);
    if (i >= 0) {
        return 1; //Отказ дублирование ключей
    }
    if (t->csize >= t->msize) {
        int f = D_Reorganization(t);
        if (t->msize <= f) {
            return 2; //Отказ переполнение
        }
    }
    t->ks[t->csize].key = k;
    t->ks[t->csize].par = par;
    t->ks[t->csize].len = strlen(info) + 1; //включая нуль-байт
    fseek(t->fd, 0, SEEK_END);// позиционирование на конец файла
    t->ks[t->csize].offset = ftell(t->fd); //Запись в таблицу: смещения информации в файле
    t->ks[t->csize].busy = 1;
    fwrite(info, sizeof(char), t->ks[t->csize].len, t->fd);
    t->csize++;
    return 0; //Good
}

int D_Delete(Table *t) {
    int k, n;
    puts("Enter key, which you want to delete -->");
    n = getInt(&k);
    if (n == 0) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    int i = find(t, k);
    if (i >= 0)
        delete2(t, k, i);
    if (i < 0) {
        puts("This key was not found\n");
        return -1; //Такого ключа нет
    }
    return 1; //Good
}

int D_Find(Table *t) {
    int k, n, i;
    char *info = NULL;
    puts("Enter the key you want to find -->");
    n = getInt(&k);
    if(n == 0) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    info = findInfo(t, k);
    if (info && t->ks[i].busy == 1) {
        printf("table[%d] = info \"%s\"\n", k, info);
        free(info);
    }
    else {
        printf("Item %d was not found\n", k);
        return -1; //Не найден
    }
}

int find(const Table *t, int k) { //Функция поиска в таблице элемента заданного ключом
    for(int  i = 0; i < t->csize; ++i)
        if(t->ks[i].key == k && t->ks[i].busy == 1)
            return i;
    return -1;
}

int D_Show(Table *t) {
    puts("busy ||| key ||| parentKey |||   info");
    for (int i = 0; i < t->csize; ++i) {
        printf("%d \t %d \t %d \t\t", t->ks[i].busy, t->ks[i].key, t->ks[i].par);
        fseek(t->fd, t->ks[i].offset, SEEK_SET);
        char buf[1024];
        fgets(buf, t->ks[i].len, t->fd);
        printf("%s\n", buf);
    }
    return 1;
}

int findParent(const Table *t, int k) {
    for (int i = 0; i < t->csize; ++i) {
        if (k == t->ks[i].par && t->ks[i].busy == 1)
            return i;
    }
    return -1; //-1 Не нашли
}


char *findInfo(const Table *t, int k) { //Функция поиска в таблице информации для элемента, заданного ключом
    char *info = NULL;
    int i = find(t, k);
    if (i >= 0){
        info = (char *) malloc(t->ks[i].len);
        fseek(t->fd, t->ks[i].offset, SEEK_SET);
        fread(info, sizeof(char), t->ks[i].len, t->fd);
    }
    return info;
}

int D_Reorganization(Table *t) {
    int j = reorganize(t);
    if (j >= t->msize) {
        puts("The table was reorganized successfully, but the table is still full\n");
        return -1;
    }
    puts("Table reorganization is successful\n");
    return 1;
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
    fread(&t->csize, sizeof(int), 1, t->fd);
    fread(t->ks, sizeof(KeySpace), t->csize, t->fd);
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
    // Записываем в начало файла размер
    fwrite(&t->msize, sizeof(int), 1, t->fd);
    // Размер таблицы
    fwrite(&t->csize, sizeof(int), 1, t->fd);
    // Саму таблицу
    fwrite(t->ks, sizeof(KeySpace), t->msize, t->fd);
    return 1;
}

int D_Load(Table *t) {
    int SZ;
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = getStr("");
    if(fname == NULL)
        return 0;
    if (load(t, fname) == 0){ // Указанный файл не сушествует: нужно создать новый файл и новую таблицу
        printf("Enter possible vector size: -->");
        if (getInt(&SZ) == 0)
            return 0;
        create(t, fname, SZ);
    }
    free(fname);
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


int to_int(char *s) {
    int i = strlen(s), j = 0, ans = 0;
    while (j != i) {
        ans = (ans * 10) + (s[j] - '0');
        ++j;
    }
    return ans;
}

int delTable(Table *t) {
//    for (int i = 0; i < t->csize; ++i) {
////        if (t->ks[i]nfo) {
////            //free(t->ks[i].data->info);
////           free(t->ks[i].data);
////        }
//    }
//    //free(t->ks->data);
    free(&t->ks[0]);
    free(t);
}

int reorganize(Table *t) {
    int i = 0, j = 0, len = t->csize;
    while (i < len) {
        if (t->ks[i].busy == 1) {
            t->ks[j] = t->ks[i];
            j++;
        }
        else t->csize--;
        ++i;
    }
    return j; //<---это последний +1 элемент
}

int delete(Table *t, int k, int i) {
    t->ks[i].busy = 0;
    int j = findParent(t, t->ks[i].key);
    if (j != -1) delete(t, t->ks[j].key, j);
}

int delete2(Table *t, int k, int i) {
    t->ks[i].busy = 0;
    int j = findParent(t, t->ks[i].key);
    while (j != -1) {
        delete(t, t->ks[j].key, j);
        j = findParent(t, t->ks[j].key);
    }
}

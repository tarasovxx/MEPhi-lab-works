#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h" //UNION
#include "error.h"
#include "hash.h"
#include <readline/readline.h>

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
    return 1;
}

int findKey(Table *t, const char *k) {
    if (t->csize < 1) return -1;
    int j = hash(k) % t->msize;
    int n = 0;
    while (t->ks[j].busy != 0 && n < t->msize) {
        if (t->ks[j].busy == 1 && hash(k)== hash(t->ks[j].key)) return j;
        int p = t->msize % 2 == 0 ? 3 : 2;
        j = (j + p) % t->msize;
        n++;
    }
    return -1; //-1 Не нашли
}

int insert(Table *t, char* k, int info) {
    if (t->csize >= t->msize) return 2; //таблица заполнена
    int j = (hash(k) % t->msize);
    int rel = 0;
    int n = 0; //Кол-во просмотренных элементов
    while (t->ks[j].busy == 1 && n < t->msize) {
        int p = t->msize % 2 == 0 ? 3 : 2;
        if (t->ks[j].busy == 1) rel++;
        j = (j + p) % t->msize;
        n++;
    }
    if (n < t->msize) {
       	t->ks[j].key = k;
        t->ks[j].busy = 1;
        t->ks[j].release = rel;
        t->ks[j].data->x = info;
        t->csize++;
    }
    return 0; //Good
}

int D_Delete(Table *t) {
    int n;
    puts("Enter key, which you want to delete -->");
    scanf("%*c");
    char *k = getStr("");
    if(k == NULL) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    int i = findKey(t, k);
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
}

int D_Show(Table *t) {
    puts("busy ||| key    |||  release ||| info");
    for (int i = 0; i < t->msize; ++i) {
        if (t->ks[i].key)
            printf("%d \t %s \t\t %d \t %d\n", t->ks[i].busy, t->ks[i].key, t->ks[i].release, t->ks[i].data->x);
    }
    return 1;
}

//int D_Reorganization(Table *t) {
//    int j = reorganize(t);
//    if (j >= t->msize) {
//        puts("The table was reorganized successfully, but the table is still full\n");
//        return -1;
//    }
//    puts("Table reorganization is successful\n");
//    return 1;
//}


int D_CheckRelease(Table *t) {
    Table *newTable = (Table *) calloc(1, sizeof(Table));
    newTable->ks = calloc(10, sizeof(KeySpace));

    int i = 0;
    puts("Enter key: -->");
    scanf("%*c");
    char *key = getStr("");
    if (key == NULL) return 0;
    int j = hash(key) % t->msize;
    int n = 0;
    while (t->ks[j].busy == 1 && n < t->msize) {
        int p = t->msize % 2 == 0 ? 3 : 2;
        if (t->ks[j].busy == 1 && hash(key) == hash(t->ks[j].key)) {
            newTable->ks[i++] = t->ks[j];
            newTable->msize++;
            newTable->csize++;
        }
        j = (j + p) % t->msize;
        n++;
    }
    puts("****************************************************");
    D_Show(newTable);

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
    for (int i = 0; i < t->msize; ++i) { //Возможна утечка при реорганизации
        //if (t->ks[i].data->info) {
        //free(t->ks[i].data->info);
        free(t->ks[i].data);
		if (t->ks[i].key) free(t->ks[i].key);
        //free(&t->ks[i]);
        //}
    }
    //free(t->ks->data);
    free(t->ks);
    free(t);
}

int delete(Table *t, const char *k, int i) {
    //int i = findKey(t, k);
    if (i  < t->msize && i > -1)
        t->ks[i].busy = 0;
    //int j = findParent(t, t->ks[i].key);
    //if (j != -1) delete(t, t->ks[j].key, j);
//    if (t->ks[i].par == 0) return 0;
//    if (t->ks[i].par != 0) {
//        int j = find(t, t->ks[i].par, 1);
//        delete(t, t->ks[j].par, j);
//    }
}

//int delete2(Table *t, int k, int i) {
//    t->ks[i].busy = 0;
//    int j = findParent(t, t->ks[i].key);
//    //if (i == j) j = -1;
//    while (j != -1) {
//        delete(t, t->ks[j].key, j);
//        j = findParent(t, t->ks[j].key);
//    }
//}

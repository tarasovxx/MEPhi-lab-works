#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userFunc.h"
#include "hashTable.h"
#include "checkFunc.h"
#include "error.h"
#include "hash.h"

#define SEP " "


int dialog(const char *msgs[], int n) {
    char *errmsg = "";
    int rc;
    int i, n1;
    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeate, please!";
        for(i = 0; i < n; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");
        n1 = getInt(&rc);
        if(n1 == 0) rc = 0;
    } while(rc < 0 || rc >= n);
    return rc;
}

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
    printf("The item table[%s] was successfully deleted\n", k);
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
    printf("Good! Element found! table[%s]\n", k); //Всё гуд, найден
    free(k);
}

int D_Show(Table *t) {
    puts("busy ||| key    |||  release ||| info");
    for (int i = 0; i < t->msize; ++i) {
        fseek(t->fd, t->ks[i].offset, SEEK_SET);
        char buf[1024] = {'\0'};
        fgets(buf, t->ks[i].len, t->fd);
        char *tok = strtok(buf, SEP);
        char *key = tok;
        tok = strtok(NULL, SEP);
        char *infoChar = tok;
        int infoInt = 0;
        if (infoChar != NULL) infoInt = atoi(infoChar);
        if (t->ks[i].busy)
            printf("%d \t %s \t\t %d \t %d\n", t->ks[i].busy, key, t->ks[i].release, infoInt);
        else
            printf("%d \t %s \t\t %d \t %d\n", t->ks[i].busy, key, t->ks[i].release, infoInt);
    }
    return 1;
}

int D_CheckRelease(Table *t) {
    puts("Please select: \n1 - Find all versions of the item; \n2 - Find a single one;");
    int checker,userSelect;
    checker = getInt(&userSelect);
    if (checker == 0) return 0;
    //Table *newTable = (Table *) calloc(1, sizeof(Table));
    //int i = 0, sizeNewTable = 10;
    //newTable->ks = calloc(sizeNewTable, sizeof(KeySpace));
    //newTable->rel = calloc(sizeNewTable, sizeof(Pair));
    puts("Enter key: -->");
    scanf("%*c");
    char *key = getStr("");
    puts("***************************************************************************************");
    if (key == NULL) return 0;
    if (userSelect == 1) {
        int j = hash(key) % t->msize;
        int n = 0;
        while (t->ks[j].busy == 1 && n < t->msize) {
            int step = primeNumber(t->msize);//t->msize % 2 == 0 ? 3 : 2;
            fseek(t->fd, t->ks[j].offset, SEEK_SET);
            char buf[1024] = {0};
            fgets(buf, t->ks[j].len, t->fd);
            char *tok = strtok(buf, SEP);
            char *kk = tok;
            tok = strtok(NULL, SEP);
            char *infoChar = tok;
            int infoInt = 0;
            if (infoChar) infoInt = atoi(infoChar);
            if (kk && t->ks[j].busy == 1 && hash(key) == hash(kk)) {
//                //newTable->ks[i++] = t->ks[j];
//                newTable->ks[i].busy = t->ks[j].busy;
//                newTable->ks[i].len = t->ks[j].len;
//                newTable->ks[i].offset = t->ks[j].offset;
//                //newTable->ks[i].key = strdup(t->ks[j].key);
//                newTable->ks[i].release = t->ks[j].release;
                if (t->ks[j].busy)
                    printf("%d \t %s \t\t %d \t %d\n", t->ks[j].busy, key, t->ks[j].release, infoInt);
                //newTable->ks[i].data = malloc(sizeof(Item));
                //newTable->ks[i++].data->x = t->ks[j].data->x;
                //newTable->csize++;
                //newTable->msize++;
//                if (newTable->csize >= sizeNewTable) {
//                    sizeNewTable *= 2;
//                    newTable->ks = (KeySpace *) realloc(newTable->ks, sizeof(KeySpace) * sizeNewTable);
//                }
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
            fseek(t->fd, t->ks[j].offset, SEEK_SET);
            char buf[1024] = {0};
            fgets(buf, t->ks[j].len, t->fd);
            char *tok = strtok(buf, SEP);
            char *kk = tok;
            tok = strtok(NULL, SEP);
            char *infoChar = tok;
            int infoInt = 0;
            if (infoChar) infoInt = atoi(infoChar);
            if (t->ks[j].busy)
                printf("%d \t %s \t\t %d \t %d\n", t->ks[j].busy, key, t->ks[j].release, infoInt);
            //newTable->ks[i].busy = t->ks[j].busy;
            //newTable->ks[i].len = t->ks[j].len;
            //newTable->ks[i].offset = t->ks[j].offset;
            //newTable->ks[i].key = strdup(t->ks[j].key);
            //newTable->ks[i].release = t->ks[j].release;
//            newTable->ks[i].data = malloc(sizeof(Item));
//            newTable->ks[i++].data->x = t->ks[j].data->x;
            //newTable->csize++;
            //newTable->msize++;
        }


    }
    //puts("****************************************************");
    //D_Show(newTable);
    //free(newTable->ks);
    //free(newTable);
    //delTable(newTable);
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
        //t->ks[i].key = s;
        t->ks[i].release = rel;
        //t->ks[i].data = calloc(1, sizeof(Item));
        //t->ks[i].data->x = k;
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

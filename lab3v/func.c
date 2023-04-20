    #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "table.h"

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

int getInt(int *n) {
    while(1) {
        int m = scanf("%d", n);
        if (m == -1) {
            printf("\nExit!\n");
            return -1;
        }
        if (m != 1) { // || (*n < 0);
            printf("You are wrong, repeat, please!\n");
            scanf("%*[^\n]");
        }
        else {
            scanf("%*[^\n]");
            return 1;
        }
    }
}


char *getStr(const char *ms) {
    char buf[80] = {}; //Временная строка
    char *res = NULL; //Итоговая строка
    int len = 0, status = 0;
    puts(ms);
    do {
        status = scanf("%80[^\n]", buf);
        if (status < 0) {
            printf("\n");
            return NULL;
        }
        else if (status > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = (char*) realloc(res, (str_len + 1) * sizeof(char)); //Расширяем массив под определённую длину
            if (res == NULL) {
                printf("Memory error!\n");
                return NULL;
            }
            for (int i = 0; i < chunk_len; ++i) {
                res[str_len - chunk_len + i] = buf[i];
            }
            //mcp(res, buf, chunk_len); // Копирование очередных chunk_len символов в res из buf (из временного в постоянный)
            len = str_len;
        }
        else {
            scanf("%*c");
        }
    } while (status > 0);
    if (len > 0) {
        res[len] = '\0';
    }
    else {
        res = (char*) calloc(1, sizeof(char));
    }
    return res;
}

char *getFileStr(FILE *in) {
    char buf[80] = {0}; //Временная строка
    char *res = NULL; //Итоговая строка
    int len = 0, status = 0;
    do {
        status = fscanf(in, "%80[^\n]", buf);
        if (status < 0) {
            printf("\n");
            return NULL;
        }
        else if (status > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = (char*) realloc(res, (str_len + 1) * sizeof(char)); //Расширяем массив под определённую длину
            if (res == NULL) {
                printf("Memory error!\n");
                return NULL;
            }
            for (int i = 0; i < chunk_len; ++i) {
                res[str_len - chunk_len + i] = buf[i];
            }
            //mcp(res, buf, chunk_len); // Копирование очередных chunk_len символов в res из buf (из временного в постоянный)
            len = str_len;
        }
        else {
            fscanf(in, "%*c");
        }
    } while (status > 0);
    if (len > 0) {
        res[len] = '\0';
    }
    else {
        res = (char*) calloc(1, sizeof(char));
    }
    return res;
}


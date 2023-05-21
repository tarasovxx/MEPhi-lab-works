#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkFunc.h"

// Функция ввода целого числа с проверкой правильности ввода.
// Число должно размещаться на отдельной строке.
// При вводе числа весь остаток строки удаляется из буфера ввода.
// При обнаружении конца файла функция возвращает 0
int getInt(int *pn) {
    const char *errmsg = "";
    int n;
    do {
        puts(errmsg);
        errmsg = "Illegal integer; enter once more";
        n = scanf("%d", pn); //sizeof(int)
        if (n < 0)	// обнаружен конец файла
            return 0;
        scanf("%*[^\n]"); // игнорирование всех символов до конца строки
        scanf("%*c");	// игнорирование символа конца строки
    } while (n == 0);
    return 1;
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

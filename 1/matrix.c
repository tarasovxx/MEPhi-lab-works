#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int input(matrix *mn) {
    const char *pr = "";
    int m = 0;
    do {
        printf("%s\n", pr);
        printf("Enter number of lines--->");
        pr = "You are wrong, repeat, please!";
        if (getInt(&m) == -1) return 0;
    } while (m < 1);
    mn->size = m;
    mn->data = (line *) calloc(m, sizeof(line));
    for (int i = 0; i < mn->size; ++i) {
        pr = "";
        do {
            printf("%s\n", pr);
            printf("Enter number of items in line %d --->", i + 1);
            pr = "You are wrong, repeat, please!";
            if (getInt(&m) == -1) {
                mn->size = i;
                erase(mn);
                return 0;
            }

            mn->data[i].size = m;
            mn->data[i].data = (int *) malloc(m * sizeof(int));
            for (int k = 0; k < m; ++k) {
                printf("Enter mas[%d] element of the line %d \n--->", k, i + 1);
                int el;
                if (getInt(&el) == 1) mn->data[i].data[k] = el;
            }
        } while (m < 1);
    }
}



//int getInt(int *n) {
//    return scanf("%d", n);
//}
int getInt(int *n) {
    while(1) {
        int m = scanf("%d", n);
        if (m == -1) {
            printf("\nExit!\n");
            return -1;
        }
        if (m != 1 || (*n < 0)) {
            printf("You are wrong, repeat, please!\n");
            scanf("%*[^\n]");
        }
        else return 1;
    }
}

void output(const char *msg, matrix mn) {
    printf("%s", msg);
    for (int i = 0; i < mn.size; ++i) {
        for (int j = 0; j < mn.data[i].size; ++j) {
            printf("%d ", mn.data[i].data[j]);
        }
        printf("\n");
    }
}

void erase(matrix *mn) {
    for (int i = 0; i < mn->size; ++i) {
        free(mn->data[i].data);
    }
    free(mn->data);
}

int task(matrix *mn) {
    for (int i = 0; i < mn->size; ++i) {
        line ptr = mn->data[i];
        int ind_i = indI(&ptr);
        int ind_j = indJ(&ptr);
        swapInd(&ptr, ind_i, ind_j);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if defined (queue_vector)
#include "queue_vector.h"  //gcc ------- -D queue_list -o lab2
#elif defined (queue_list)
#include "queue_list.h"
#endif

#define DELIM " "
#include "func.h"


////Здесь находятся общие функции
void reading (Item *tmp, int racks, Queue *mas, int len, int capacity) {
    for (int i = 0; i < racks; ++i) {
        mas[i] = *queue_new(capacity);
    }
//    for (int j = 0; j < len; ++j) {
//        queue_push(&mas[j % racks], &tmp[j]);
//    }
    //Item *new = malloc(sizeof(Item));
    Item *new = malloc(sizeof(Item));
    Item *new1 = malloc(sizeof(Item));
    int i = 0;
    int *tl = (int *) calloc(racks, sizeof(int));
    while (i < len) {
        Item *t = &tmp[i]; //Имеет тот же адрес
        if (mas[i % racks].n <= mas[i % racks].capacity) queue_push(&mas[i % racks], t);
        if (tl[i % racks] == 0) {
            //Item *new = malloc(sizeof(Item));
            queue_front(&mas[i % racks], &new);
            tl[i % racks] = new->ta + new->ts;
            //free(new);
        }
        if (i >= racks - 1) {
            for (int j = 0; j < racks; ++j) {
                if (tl[j] <= t->ta) {
                    queue_pop(&mas[j]);
                    //Item *new1 = malloc(sizeof(Item));
                    int f = queue_front(&mas[j], &new1);
                    if (f == 1) tl[j] = 0;
                    else tl[j] += new1->ts;
                    //free(new1);
                }
            }
        }
        if ((i == len - 1 || i < len - 1) && tmp[i + 1].ta != t->ta) {
            printf("Moment #%d\n", t->ta);
            for (int j = 0; j < racks; ++j) {
                printf("#%d \t", j + 1);
                queue_print(&mas[j % racks]);
            }
        }
        ++i;

    }
    free(tl);
    //if (new) free(new);
    //if (new1) free(new1);

}

int cmp_moment(const Item *p1, const Item *p2) {
    return p1->ta - p2->ta;
}


void input(char *s, Item **arr, int *len) {
    char *istr = strtok(s, DELIM);
    *arr = malloc(10 * sizeof(Item));

    while (istr) {
        int ta = 0, ts = 0, i = 0;
        int l = strlen(istr);
        char id = istr[0];
        i = 2;
        if (istr[i] >= '0' && istr[i] <= '9') {
            while(istr[i] != '/') {
                ta = ta * 10 + (int)(istr[i] - '0');
                ++i;
            }
            ++i;
            if (istr[i] >= '0' && istr[i] <= '9') {
                while (i < l && istr[i] != '\n') {
                    ts = ts * 10 + (int)(istr[i] - '0');
                    ++i;
                }
                (*arr)[(*len)].id = id; (*arr)[(*len)].ta = ta; (*arr)[(*len)].ts = ts;
                (*len)++;
                if ((*len) > 10) {
                    *arr = realloc(*arr, ((*len) * 2) * sizeof(Item));
                }
            }
        }
        istr = strtok(NULL, DELIM);
    }
    *arr = realloc(*arr, (*len) * sizeof(Item));
    qsort(*arr, (*len), sizeof(Item), (int (*)(const void *, const void *))cmp_moment);
}


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



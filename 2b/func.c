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


#define DELIM " "
#define INF 100000

////Здесь находятся общие функции

//int perepoln(Item *tmp, int racks, Queue *mas, int len, int capacity)

void reading (Item *tmp, int racks, Queue *mas, int len, const int capacity) {
    for (int i = 0; i < racks; ++i) {
        //mas[i] = *queue_new(capacity);
        //if (defined(queue_vector)) {
        queue_auto(&mas[i], capacity);
       	//mas[i].capacity = capacity;
        //mas[i].mas = (Item *) malloc(capacity * sizeof(Item));
    }
    Item *new = malloc(sizeof(Item));
    Item *new1 = malloc(sizeof(Item));
    int i = 0, flag = 0, moment = 0, mintl = 10000, met = 0, ind = 0, d = 0;
    int *tl = (int *) calloc(racks, sizeof(int));
    do {
        int moment = countMoment(mas, tmp, i, len, racks, &ind, &met, tl, mintl, capacity);
        checker(mas, tl, new, i, racks, &mintl);
        balance(mas, tl, new1, tmp, i, racks, &ind, &met, moment, &mintl, len);
        print(mas, tmp, moment, racks, i);
        ++i;
        flag = 0;
        d = checkEmpty(mas, racks);
        if (met == 1) d = 0;
    } while (!d); //2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9
    free(tl);
    if (new) free(new);
    if (new1) free(new1);
    //for (int i = 0; i < len; ++i) free(&tmp[i]);
    free(tmp);
}

int checkEmpty(Queue *mas, int racks) { //Выводит 1, если всё пустое, то что нам нужно
	for (int i = 0; i < racks; ++i) {
		if (!queue_empty(&mas[i])) return 0;
	}
	return 1;
	
}

int print(Queue *mas, Item *tmp, int moment, int racks, int i) {
    //if (tmp[i + 1].ta != moment) {
    //int x = tmp[i].ts;
    printf("Moment #%d\n", moment);
    for (int j = 0; j < racks; ++j) {
        printf("#%d \t", j + 1);
        queue_print(&mas[j % racks]);
    }
    // }
}

int balance(Queue *mas, int *tl, Item *new1, Item *tmp, int i, int racks, int *ind, int *met, int moment, int *mintl, int len) {
    if (i >= racks - 1) {
        for (int j = 0; j < racks; ++j) {
            if (tl[j] <= moment) { //t->ta
                Item trr;
                queue_pop(&mas[j], &trr);
                //Item *new1 = malloc(sizeof(Item));
                int f = queue_front(&mas[j], &new1);
                if (f == 1) tl[j] = INF;
                else tl[j] += new1->ts;
                int as = INF;
                for (int m = 0; m < racks; ++m) {
                    if (tl[m] < as /*|| (*met == 1 && tmp[*ind].ta < as)*/) {
                        as = tl[m];
                    }
                }
                *mintl = as;
                if (*met == 1 && moment >= tmp[*ind].ta) {
                    queue_push(&mas[j], &tmp[*ind]);
                    if (*ind < len - 1) (*ind)++;
                    else *met = 0;
                }
            }
        }
    }
}


int countMoment(Queue *mas, Item* tmp, int i, int len, int racks, int *ind, int *met, int *tl, int mintl, int c) {
    Item *t = &tmp[i];
    int moment = 0;
    if (i < len) {
        //t = &tmp[i]; //Имеет тот же адрес
        moment = t->ta;
    }
    if (i < len) {
        if (mas[i % racks].n < c) queue_push(&mas[i % racks], t); //mas[i % racks].n <= mas[i % racks].capacity
        else {
            *met = 1;
            *ind = i; //Запоминаем, что начиная с этого индекса эементы ждут освобождения очередей
        }
        if (tl[i % racks] == INF) tl[i % racks] = 0;
    }
    if (i >= len || *met == 1) {
        moment = mintl;
    }
    return moment;
}

int checker(Queue *mas, int *tl, Item *new, int i, int racks, int *mintl) {
    if (tl[i % racks] == 0) {
        int j = queue_front(&mas[i % racks], &new);
        if (j != 1) tl[i % racks] = new->ta + new->ts;
        if (tl[i % racks] < *mintl) *mintl = tl[i % racks];
    }
}

int cmp_moment(const Item *p1, const Item *p2) {
    return p1->ta - p2->ta;
    // if (m >= 0) return 1;
    // else if (m < 0) return -1;
    // else return 0;
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



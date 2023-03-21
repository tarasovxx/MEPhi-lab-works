#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h" //UNION

int D_Add(Table *t) {
    int k, rc, n, par;
    puts("Enter key: -->");
    n = getInt(&k);
    if(n == 0) return 0;
    puts("Enter par (If you don't want to specify enter 0): --->");
    n = getInt(&par);
    if (n == 0) return 0;
    while (par != 0 && find(t, par, 0) < 0) {
        puts("There is no such key in the table yet, please enter a new one, or 0\n");
        n = getInt(&par);
        if (n == 0) return 0;
    }
    scanf("%*c");
    char *info = getStr("Enter info: ");
    if(info == NULL) return 0;
    rc = insert(t, k, par, info);
    //free(info);
    if(rc)
        puts("Ok\n");
    else
        printf("Duplicate key: %d\n", k);
    return 1;
}

int find(Table *t, int k, int f) { //Можно сделать Search && find отдельными функциями
    if (t->csize == 0) return -1;
    //if (t->csize < 0) return -1; //Не нашли
    for (int i = 0; i < t->csize; ++i) {
        if (f == 0) if (k == t->ks[i].key && t->ks[i].busy == 1) return i;
        if (f == 1) if (k == t->ks[i].par) return i;
    }
    return -1; //-1 Не нашли
}


int insert(Table *t, int k, int par, char* info) {
    int i = find(t, k, 0);
    if (i >= 0) {
        puts("Duplicate keys\n");
        return -1; //Отказ дублирование ключей
    }
    if (t->csize >= t->msize) {
        int f = D_Reorganization(t);
        if (t->msize <= f) {
            puts("Overflow\n");
            return -1; //Отказ переполнение
        }
    }
    //t->ks[t->csize].data->ind = 0;
    t->ks[t->csize].key = k;
    t->ks[t->csize].par = par;
    t->ks[t->csize].data = calloc(1, sizeof(Item));
    //t->ks[t->csize].data->info = calloc(40, sizeof(char));
    t->ks[t->csize].data->info = info;
    //t->ks[t->csize].data->key = k;
    //t->ks[t->csize].data->ind = t->csize;
    t->ks[t->csize].busy = 1;
    t->csize++;
    return 1; //Good
}

int D_Delete(Table *t) {
    int k, n;
    puts("Enter key, which you want to delete -->");
    n = getInt(&k);
    if(n == 0) return 0;
    int i = find(t, k, 0);
    if (i >= 0)
        delete(t, k, i);
    if (i < 0) return -1; //Такого ключа нет
//    int i = find(t, k);
//    if (i < 0) return -1; //Такого ключа нет
//    t->ks[i].busy = 0;
    printf("The item was successfully deleted");
    return 1; //Good
}

int D_Find(Table *t) {
    int k, n;
    puts("Enter the key you want to find -->");
    n = getInt(&k);
    if(n == 0) return 0;
    int i = find(t, k, 0);
    if (i < 0 || t->ks[i].busy == 0){
        printf("Element not found\n");
        return -1; //Не найден
    }
    printf("Good! Element found! \n"); //Всё гуд, найден
}

int D_Show(Table *t) {
    for (int i = 0; i < t->csize; ++i) {
            if (t->ks[i].busy == 1)
                printf("mas[%d] = %s;\n", t->ks[i].key, t->ks[i].data->info);
    }
    return 1;
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
    fscanf(in, "%d %d", &n, &c);
    fscanf(in, "%*c");
    t->msize = n;
    t->csize = c;
    t->ks = realloc(t->ks, n * sizeof(KeySpace));
    t->ks[i].data = calloc(1, sizeof(Item));
    //t->ks[i].data->info = calloc(40, sizeof(char));

    while (!feof(in)) {
        int j = 0;
        char *tmp = getFileStr(in);
        if (!tmp) break;
        char *istr = strtok(tmp, " ");
        while (istr) {
            if (j == 0) t->ks[i].busy = istr[0] - '0';
            else if (j == 1) {
                int q = to_int(istr);
                if (find(t, q, 0) >= 0) {
                    flag = 1;
                    break;
                }
                t->ks[i].key = q;
            }
            else if (j == 2) t->ks[i].par = to_int(istr);
            else t->ks[i].data->info = istr;
            j++;
            istr = strtok(NULL, " ");
        }
        //free(tmp);

//        fscanf(in, "%*c");
//        fscanf(in, "%d%d%d", &t->ks[i].busy, &t->ks[i].key,
//               &t->ks[i].par);
//        fscanf(in, "%*c");
//        fscanf(in, "%s\n", t->ks[i].data->info);
        //t->csize++;
        //int k = t->ks[i].key;
//        int s = find(t, t->ks[i].key, 0);
//        if (s >= 0) {
//             continue;//Отказ дублирование ключей
//        }
        if (t->csize >= t->msize) {
            int f = D_Reorganization(t);
            if (t->msize <= f) {
                puts("Overflow\n");
                return -1; //Отказ переполнение
            }
        }
        //t->csize++;
        if (flag == 0) { 
        	++i;
	        t->ks[i].data = calloc(1, sizeof(Item));
	        //t->ks[i].data->info = calloc(40, sizeof(char));
	    }
        flag = 0;
        //free(tmp);
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
//    for (int i = 0; i < t->msize; ++i) {
//        Item *s = t->ks[i].data;
//        if (t->ks[i].data->info) free(t->ks[i].data);
//    }
	for (int i = 0; i < t->csize; ++i) {
	        if (t->ks[i].data->info) {
	       	  free(t->ks[i].data->info);
	  	      free(t->ks[i].data);
	    }
	}
	    //free(t->ks->data);
	free(&t->ks[0]);
	free(t);
	// free(t->ks->data->info);
    // free(t->ks->data);
    // free(&t->ks[0]);
    // free(t);
}

int reorganize(Table *t) {
    int i = 0, j = 0, len = t->csize;
    while (i < len) {
        if (t->ks[i].busy == 1) {
            t->ks[j] = t->ks[i];
            j++;
            //t->csize--;
        }
        ++i;
    }
    return j; //<---это последний +1 элемент
}

int delete(Table *t, int k, int i) {
    t->ks[i].busy = 0;
    int j = find(t, t->ks[i].key, 1);
    if (j != -1) delete(t, t->ks[j].key, j);
//    if (t->ks[i].par == 0) return 0;
//    if (t->ks[i].par != 0) {
//        int j = find(t, t->ks[i].par, 1);
//        delete(t, t->ks[j].par, j);
//    }
}

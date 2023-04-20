#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h" //UNION
#include "error.h"

int D_Add(Table *t) {
    int k, rc, n, par;
    puts("Enter key: -->");
    n = getInt(&k);
    if(n == 0) return 0;
    puts("Enter par (If you don't want to specify enter 0): --->");
    n = getInt(&par);
    if (n == 0) return 0;
    while ((par != 0) && (findKey(t, par) < 0)) {
        if (par == k) break;
        puts("There is no such key in the table yet, please enter a new one, or 0\n");
        n = getInt(&par);
        if (n == 0) return 0;
    }
    scanf("%*c");
    char *info = getStr("Enter info: ");
    if(info == NULL) return 0;
    rc = insert(t, k, par, info);
    //free(info);
    printf(errMsgs[rc]);
    //if (rc == 0) {
    //    printf(errMsgs[OK]);
    //}
    //else if (rc == 1) {
    //    printf(errMsgs[DUPLICATE_KEY]);
    //    printf(" :%d\n", k);
    //}
    //else {
    //    printf(errMsgs[TABLE_OVERFLOW]);
    //}
    return 1;
}

int findKey(Table *t, int k) { //Можно сделать Search && find отдельными функциями
    if (t->csize == 0) return -1;
    if (t->csize < 0) return -1; //Не нашли
    for (int i = 0; i < t->csize; ++i) {
        if (k == t->ks[i].key && t->ks[i].busy == 1) return i;
    }
    return -1; //-1 Не нашли
}

int findParent(Table *t, int k) {
    if (t->csize == 0) return -1;
    if (t->csize < 0) return -1; //Не нашли
    for (int i = 0; i < t->csize; ++i) {
        if (k == t->ks[i].par && t->ks[i].busy == 1) return i;
    }
    return -1; //-1 Не нашли
}


int insert(Table *t, int k, int par, char* info) {
    int i = findKey(t, k);
    if (i >= 0) {
        //puts("Duplicate keys\n");
        return 1; //Отказ дублирование ключей
    }
    if (t->csize >= t->msize) {
        int f = D_Reorganization(t);
        if (t->msize <= f) {
            //puts("Overflow\n");
            return 2; //Отказ переполнение
        }
    }
    //t->ks[t->csize].data->ind = 0;
    t->ks[t->csize].key = k;
    t->ks[t->csize].par = par;
    t->ks[t->csize].data = calloc(1, sizeof(Item));
    t->ks[t->csize].data->info = info;
    //t->ks[t->csize].data->key = k;
    //t->ks[t->csize].data->ind = t->csize;
    t->ks[t->csize].busy = 1;
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
    int i = findKey(t, k);
    if (i >= 0)
        delete2(t, k, i);
    if (i < 0) {
        puts("This key was not found\n");
        return -1; //Такого ключа нет
    }
//    int i = find(t, k);
//    if (i < 0) return -1; //Такого ключа нет
//    t->ks[i].busy = 0;
    printf("The item table[%d] = %s was successfully deleted\n", k, t->ks[i].data->info);
    printf("Parents of the deleted item: table[%d]\n", t->ks[i].par);
    return 1; //Good
}

int D_Find(Table *t) {
    int k, n;
    puts("Enter the key you want to find -->");
    n = getInt(&k);
    if(n == 0) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    int i = findKey(t, k);
    if (i < 0 || t->ks[i].busy == 0){
        printf("Element not found\n");
        return -1; //Не найден
    }
    printf("Good! Element found! table[%d] = %s \n", k, t->ks[i].data->info); //Всё гуд, найден
}

int D_Show(Table *t) {
    puts("busy ||| key ||| parentKey |||   info");
    for (int i = 0; i < t->csize; ++i) {
        //if (t->ks[i].busy == 1)
        printf("%d \t %d \t %d \t\t %s\n", t->ks[i].busy, t->ks[i].key, t->ks[i].par, t->ks[i].data->info);
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
    //fscanf(in, "%*c");
    //fscanf(in, "%d", &c);
    fscanf(in, "%*c");
    char s[80] = {};
    t->msize = n;
    t->ks = (KeySpace *) realloc(t->ks, (n + 1) * sizeof(KeySpace));
    int b, k, p;
    while(i < c) {
        fscanf(in, "%d %*c %d %*c %d %*c %[^\n]s %*c", &b, &k, &p, s);
        t->ks[i].busy = b;
        if (findKey(t, k) >= 0) {
            flag = 1;
            break;
        }
        t->ks[i].key = k;
        t->ks[i].par = p;
        t->ks[i].data = calloc(1, sizeof(Item));
        t->ks[i].data->info = calloc(40, sizeof(char));
        int len = strlen(s);
        for (int d = 0; d < len; ++d)
            t->ks[i].data->info[d] = s[d];     
        t->csize = i;
        if (flag == 0) ++i;
        flag = 0;
    }
    t->csize = c;
    free(path);
    fclose(in);
    return 1;
}

/*int D_Import(Table *t) {
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
    t->msize = n;
    //t->csize = c;
    t->ks = (KeySpace *) realloc(t->ks, (n + 1) * sizeof(KeySpace));
    char *s;
    int b, k, p;

	while(fscanf(in, "%*[^,]%d%d%d%*c", &b, &k, &p, s)) {
		t->ks[i].busy = b;
		if (findKey(t, k) >= 0) {
			flag = 1;
			break;
		}
		t->ks[i].key = k;
		t->ks[i].par = p;
		t->ks[i].data = calloc(1, sizeof(Item));
		t->ks[i].data->info = calloc(40, sizeof(char));
		t->ks[i].data->info = s;
	

    
    /*while (!feof(in)) {
        int j = 0;
        //fscanf(in, "%[^\n]", tmp);
        char *tmp = getFileStr(in);
        int sdf = strlen(tmp);
        //printf("%d", sdf);
        //tmp[sdf] = '\0';
        //char *stmp = strdup(tmp);
        char *istr = strtok(tmp, ",");
        while (istr) {
            if (j == 0) t->ks[i].busy = istr[0] - '0';
            else if (j == 1) {
                int q = to_int(istr);
                if (findKey(t, q) >= 0) {
                    flag = 1;
                    break;
                }
                t->ks[i].key = q;
            }
            else if (j == 2) t->ks[i].par = to_int(istr);
            else {
            	t->ks[i].data = calloc(1, sizeof(Item));
                t->ks[i].data->info = calloc(40, sizeof(char));
            	//t->ks[i].data->info = istr;

            	
            	int sd = strlen(istr);
                for (int l = 0; l < sd; ++l)
                	t->ks[i].data->info[l] = istr[l];
            }
            j++;
            istr = strtok(NULL, ",");
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
        //if (t->csize >= t->msize) {
        //    int f = D_Reorganization(t);
        //    if (t->msize <= f) {
        //        puts("Overflow\n");
        //        return -1; //Отказ переполнение
        //    }
        //}
        t->csize = i;
        if (flag == 0) ++i;
        flag = 0;
        //t->ks[i].data = calloc(1, sizeof(Item));
        //t->ks[i].data->info = calloc(40, sizeof(char));
        //Item *ms = t->ks[1].data;
        //int jfd = 0;
        //free(tmp);
    }
    free(path);
    fclose(in);
    return 1;
}*/

int to_int(char *s) {
    int i = strlen(s), j = 0, ans = 0;
    while (j != i) {
        ans = (ans * 10) + (s[j] - '0');
        ++j;
    }
    return ans;
}

int delTable(Table *t) {
    for (int i = 0; i < t->csize; ++i) { //Возможна утечка при реорганизации
        //if (t->ks[i].data->info) {
            free(t->ks[i].data->info);
            free(t->ks[i].data);
            //free(&t->ks[i]);
        //}
    }
    //free(t->ks->data);
    free(t->ks);
    free(t);
}

int reorganize(Table *t) {
    int i = 0, j = 0, len = t->csize, ls = 0;
    while (i < len) {
        if (t->ks[i].busy == 1) {
            //t->ks[j] = t->ks[i];
            free(t->ks[j].data->info);
            *t->ks[j].data->info = *t->ks[i].data->info;
            t->ks[j].busy = t->ks[i].busy;
            t->ks[j].key = t->ks[i].key;
            t->ks[j].par = t->ks[i].par;
            j++;
            //t->csize--;
        }
        else {
        	//free(t->ks[t->csize - 1].data->info);
        	//free(t->ks[t->csize - 1].data);
        	//free(&t->ks[t->csize]);
        	//t->csize--;
        	ls++;
        }
        ++i;
    }
    //t->csize -= ls;
    for (int i = 0; i < ls; ++i) {
    	free(t->ks[t->csize - 1].data->info);
    	free(t->ks[t->csize - 1].data);
    	t->csize--;
    }
    return j; //<---это последний +1 элемент
}

int delete(Table *t, int k, int i) {
    t->ks[i].busy = 0;
    int j = findParent(t, t->ks[i].key);
    if (j != -1) delete(t, t->ks[j].key, j);
//    if (t->ks[i].par == 0) return 0;
//    if (t->ks[i].par != 0) {
//        int j = find(t, t->ks[i].par, 1);
//        delete(t, t->ks[j].par, j);
//    }
}

int delete2(Table *t, int k, int i) {
    t->ks[i].busy = 0;
    int j = findParent(t, t->ks[i].key);
    //if (i == j) j = -1;
    while (j != -1) {
        delete(t, t->ks[j].key, j);
        j = findParent(t, t->ks[j].key);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h" //UNION
#include "error.h"
#include "stack.h"

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
    free(info);
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
    if (t->csize < 1) return -1; //Не нашли
    for (int i = 0; i < t->csize; ++i) {
        if (t->ks[i].busy == 1 && k == t->ks[i].key) return i;
    }
    return KEY_NOT_FOUND; //-1 Не нашли
}

KeySpace *findNextItem(Table *t) {
	static int currentIndex = 0; // Храним текущий индекс на каждом заходе в функцию ьерем тот, который был при выходе из функции

	while (currentIndex < t->csize) {
		KeySpace *ks = &(t->ks[currentIndex]);
		if (ks->busy) {
			// Поймали какой-то элемент его ретерним и уже в функции проверям то ли это
			currentIndex++;
			return ks;
		}
	}
	currentIndex = 0;
	return NULL; // Если вернули NULL значит обошли всё, значит следующая итерация будет с начала

	
}

int findParent(Table *t, int k) {
    if (t->csize < 1) return -1; //Не нашли
    for (int i = 0; i < t->csize; ++i) {
        if (k == t->ks[i].par && t->ks[i].busy == 1) return i;
    }
    return KEY_NOT_FOUND; //-1 Не нашли
}

int findAllParent(Table *t, int *mas, int k) {
    if (t->csize < 1) return -1; //Не нашли
    int j = 0;
    for (int i = 0; i < t->csize; ++i) {
        if (k == t->ks[i].par && t->ks[i].busy == 1) mas[j++] = i;
    }

    return j;
}


int insert(Table *t, int k, int par, char* info) {
    int i = findKey(t, k);
    if (i >= 0) {
        //puts("Duplicate keys\n");
        return DUPLICATE_KEY; //Отказ дублирование ключей
    }
    if (t->csize == t->msize) {
        int f = D_Reorganization(t);
        if (f == TABLE_OVERFLOW) {
            //puts("Overflow\n");
            return TABLE_OVERFLOW; //Отказ переполнение
        }
    }
    //t->ks[t->csize].data->ind = 0;
    t->ks[t->csize].key = k;
    t->ks[t->csize].par = par;
    t->ks[t->csize].data = calloc(1, sizeof(Item));
    //t->ks[t->csize].data->info = calloc(40, sizeof(char));
    //t->ks[t->csize].data->info = info;
    t->ks[t->csize].data->info = strdup(info);
    //t->ks[t->csize].data->key = k;
    //t->ks[t->csize].data->ind = t->csize;
    t->ks[t->csize].busy = 1;
    t->csize++;
    return OK; //Good
}

int D_Delete(Table *t) {
    int k, n;
    puts("Enter key, which you want to delete -->");
    n = getInt(&k);
    if (n == 0) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    int i = findKey(t, k); //Убрать в delete 
    if (i >= 0)
        delete2(t, i);
    if (i < 0) {
        puts("This key was not found\n");
        return KEY_NOT_FOUND; //Такого ключа нет
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
        return KEY_NOT_FOUND; //Не найден
    }
    printf("Good! Element found! table[%d] = %s \n", k, t->ks[i].data->info); //Всё гуд, найден
}

int D_Show(Table *t) {
    puts("busy ||| key ||| parentKey |||   info");
    for (int i = 0; i < t->csize; ++i) {
        //if (t->ks[i].busy == 1)
        //if (i == t->csize) puts("*****************************************************************************");
        if (t->ks[i].data) printf("%d \t %d \t %d \t\t %s\n", t->ks[i].busy, t->ks[i].key, t->ks[i].par, t->ks[i].data->info);
    }
    return 1;
}

int D_Reorganization(Table *t) {
    reorganize(t);
    if (t->csize >= t->msize) {
        puts("The table was reorganized successfully, but the table is still full\n");
        return TABLE_OVERFLOW;
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
        if (t->ks[i].data) {
        	free(t->ks[i].data->info);
        	free(t->ks[i].data);
        }
        
    }
    //free(t->ks->data);
    free(t->ks);
    free(t);
}

void reorganize(Table *table) {
    int j = 0;
    for (int i = 0; i < table->csize; ++i) {
            if (table->ks[i].busy) {
                if (j == i) {
                    ++j;
                    continue;
                }
                if (table->ks[j].data) {
                	free(table->ks[j].data->info);
                	free(table->ks[j].data);
                }
                table->ks[j] = table->ks[i];
    
                //table->ks[j].busy = 1;
                //table->ks[j].key = table->ks[i].key;
                //table->ks[j].par = table->ks[i].par;
                ///table->ks[j].data = (Item *) malloc(sizeof(Item));
                //table->ks[j].data->info = strdup(table->ks[i].data->info);
                ++j;
            }
            else {    
	            table->ks[i].busy = 0;
	            free(table->ks[i].data->info);
	            free(table->ks[i].data);
	            table->ks[i].data = NULL;
	        }
        }
        //free(table->ks[j].data->info);
        
    
        table->csize = j;
}


int delete(Table *t, int i) {
    if (i == -1) return 0;
    t->ks[i].busy = 0;
    int dsf = t->ks[i].key;
    int j = 0;
    j = findParent(t, t->ks[i].key);
    delete(t, j);
    j = findParent(t, t->ks[i].key);
    delete(t, j);
    //delete(t, t->ks[j].key, j);
    return 0;
}

int delete2(Table* t, int i) {
    // Заведем стэк (Заменит стэк рекурсивных вызовов)
    // Рассматриваем случай, когда переполнение стэка невозможно, в связи с тем, что размер таблицы фиксирован
    Stack *st = stack_create(t->msize);
    stack_push(st, i);
    while (stack_empty(st) != 1) {
        // Добавляем в стэк всех детей i и помечаем i даленной
        int numb = st->top - 1;
        int ind = (numb >= 0) ? st->mas[numb] : 0;
        int *mas = calloc(t->csize, sizeof(int));
        int len = findAllParent(t, mas, t->ks[ind].key);
        t->ks[ind].busy = 0;
        int fd;
        stack_pop(st, &fd);
        while (len > 0) stack_push(st, mas[--len]); // стэк ИНДЕКСОВ!
        free(mas);
        i = st->mas[st->top];
    }
    stack_destroy(st);

    return 0;
}

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>

#include "userFunc.h"
#include "checkFunc.h"
#include "error.h"


// Функция для выбора номера альтернативы.
// Возвращает правильный номер альтернативы.
// В строке может быть указан только номер альтернативы;
// если в строке после номера альтернативы есть что-то еще,
// весь остаток строки удаляется из буфера

int dialog(const char *msgs[], int N) {
    const char *errmsg = "";
    int rc;
    int i, n;

    do {
        puts(errmsg);
        errmsg = "You are wrong. Repeat, please!";

        // вывод списка альтернатив
        for (i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");

        n = getInt(&rc); // ввод номера альтернативы
        if (n == 0) // конец файла - конец работы
            rc = 0;
    } while (rc < 0 || rc >= N);

    return rc;
}

// Диалоговая функция включения в дерево нового элемента.
// Требует ввести ключ.
// Если ключ задан неправильно, вся строка игнорируется
int D_Add(Node **proot) {
    int information, rc, n;
    //puts("Enter key [P.S.: char *]: -->");
    char *key = getStr("Enter key [P.S.: char *]: -->");
    puts("Enter information [P.S.: uint]: --->");
    n = getInt(&information);
    if (n == 0)
        return 0; // обнаружен конец файла
    *proot = insert(*proot, key, information);	// вставка элемента в таблицу
    if ((*proot)->key1)
        puts("Ok\n");
//    else
//        printf("Duplicate key: %d\n", k);
    free(key);
    return OK;
}


// Диалоговая функция поиска элемента в таблице по ключу
int D_Find(Node **proot) {
    int k, n, ord;
//    puts("Enter key: -->");
//    n = getInt(&k);
    char *key = getStr("Enter key [P.S.: char *]: -->");
    if (!key)
        return 0;
    puts("Enter release:");
    n = getInt(&ord);
    if (n == 0)
        return 0;
    Node *target = find(*proot, key, ord);
    if (target) {
        printf("key = %s\ninformation = %d\nWas found\n", target->key1->data, target->info);
    }
    else
        if (*proot) printf("Node with key %s was not found\n", key);
    free(key);
    return OK;
}

// Диалоговая функция специального поиска элемента в дереве (поиска элемента с наибольшим значением ключа)
int D_FindSpecial(Node **proot) {
    Node *src = getMax(*proot);
    char *conc = src->key1->data;
    if (src->key2) conc = src->key2->data;
    if (src) {
        printf("The element with the highest key value: %s: %u", conc, src->info);
    }
    return OK;
}

// диалоговая функция удаления из дерева элемента, заданного ключом
int D_Delete(Node **proot) {
    int n, k, ord;
    char *key = getStr("Enter key [P.S.: char *]: -->");
    if (!key)
        return ERR;
    puts("Enter release:");
    n = getInt(&ord);
    if (n == 0)
        return 0;
    Node *elem = delete(*proot, key, ord);
    if (elem) {
    	*proot = elem; 
        printf("Ok\n");
    }
    else {
		*proot = elem;
		printf("Node with key %s was not found\n", key);
	}
    free(key);
    return OK;
}

// диалоговая функция вывода содержимого таблицы
int D_Show(Node **proot) {
    printf("\n=== Your tree: ===\n\n");
    print_tree(*proot, 0);
    return OK;
}

int D_Traversal(Node **proot) {
    puts("Direct Tree Traversal In The Specifed Range");
    char *a = getStr("Please enter the specified range [a; b] |||| [P.S.: char *]\nEnter first key:");
    char *b = getStr("Enter second key:");
    if (!a || !b) {
        puts("Error input, EOF inspected");
        return ERR;
    }
    puts("Key output:");
    treeTraversal(*proot, a, b);
    free(a);
    free(b);
    return OK;
}


int D_Import(Node **proot) {
    char buffer[1024];
    char *filename = getStr("Please enter the filename:");
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file %s\n", filename);
        return ERR_FILE;
    }
    char *key = NULL;
    unsigned int info = 0;
    while (fgets(buffer, 1024, file)) {
        int len = strlen(buffer);
        if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';
        if (key == NULL) {
            key = strdup(buffer);
        }
        else {
            info = atoi(buffer);
            insert(*proot, key, info);
            key = NULL;
        }
    }
    if (key != NULL) {
        printf("Warning: missing info for key %s\n", key);
        free(key);
    }
    free(filename);
    fclose(file);
    return OK;
}

// Таймирование, аргумент функции не нужен, так как создается собственное дерево
int D_Timing(Node **) {
    Node *root = calloc(1, sizeof(Node)); // = &EList
    int n = 10, cnt = 1000000, i, m;
    char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *key[10000];
    char *k;
    clock_t first, last;
    Node **masPtr = calloc(1000, sizeof(Node *));
    srand(time(NULL));
    while (n-- > 0){
        for (i = 0; i < 10000; ++i) {
            char *k = calloc(5, sizeof(char));
            for (int j = 0; j < 4; ++j) {
                int rand_index = rand() % (sizeof(letters) - 1);
                k[j] = letters[rand_index];
            }
            key[i] = k;
        }
        for (i = 0; i < cnt; ){
            char *k = calloc(5, sizeof(char));
            for (int j = 0; j < 4; ++j) {
                int rand_index = rand() % (sizeof(letters) - 1);
                k[j] = letters[rand_index];
            }
            if (insert(root, k, rand()))
                ++i;
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i)
            if (find(root, key[i], 0)) ++m;
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %lf\n", 10 - n, (10 - n) * cnt, (double)(last - first) / CLOCKS_PER_SEC);

    }
    delTree(&root);
    return OK;
}

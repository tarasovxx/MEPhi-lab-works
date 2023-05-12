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
    rc = insert(proot, key, information);	// вставка элемента в таблицу
    if (rc)
        puts("Ok\n");
//    else
//        printf("Duplicate key: %d\n", k);
    free(key);
    return OK;
}


// Диалоговая функция поиска элемента в таблице по ключу
int D_Find(Node **proot) {
    int k, n;
//    puts("Enter key: -->");
//    n = getInt(&k);
    char *key = getStr("Enter key [P.S.: char *]: -->");
    if (!key)
        return 0;
    Node **masPtr = (Node **) calloc(50, sizeof(Node *));
    //masPtr = (Node **)calloc(i + 1, sizeof(Node *));
    int len = findAll(masPtr, *proot, key);
    masPtr = realloc(masPtr, len * sizeof(Node *));
    if (len >= 1) {
        for (size_t i = 0; i < len; ++i) {
            printf("key = %s\ninformation = %d\nWas found\n", masPtr[i]->key, masPtr[i]->info);
            puts("**************************");
        }
    }
    else
        printf("Node with key %s was not found\n", key);
    free(key);
    free(masPtr);
    return OK;
}

// Диалоговая функция специального поиска элемента в таблице по ключу, значение которого наиболее близко к азаднному
int D_FindSpecial(Node **proot) {
    int k, n;
//    puts("Enter key: -->");
//    n = getInt(&k);
    char *key = getStr("Enter key [P.S.: char *]: -->");
    if (!key)
        return 0;
    Node **masPtr = (Node **) calloc(50, sizeof(Node *));
    int i = 0;
    int len = lowerBound(masPtr, *proot, key, 0, 0, &i);
    masPtr = realloc(masPtr, len * sizeof(Node *));
    puts("Keys whose value is closest to the specified one, but does not match it");
    if (i >= 1) {
        for (size_t j = 0; j < i; ++j) {
            printf("key = %s; info = %d\n ", masPtr[j]->key, masPtr[j]->info);
        }
    }
    else
        printf("key = %s; info = %d\n ", (*proot)->key, (*proot)->info);
    free(key);
    free(masPtr);
    return OK;
}



// диалоговая функция удаления из дерева элемента, заданного ключом
int D_Delete(Node **proot) {
    int n, k;
    int rc;
    char *key = getStr("Enter key [P.S.: char *]: -->");
    if (!key)
        return 0;
    *proot = deleteNode(*proot, key);
    if (*proot)
        printf("Ok\n");
    else
        if (*proot) printf("Node with key %d was not found\n", k);
    free(key);
    return OK;
}

// диалоговая функция вывода содержимого таблицы
int D_Show(Node **proot) {
    printf("\n=== Your tree: ===\n\n");
    //printTree(*proot);
    //viewTree(*proot, 10);
    putTree(*proot, 0);

    puts("\nDirect Tree Traversal In The Specifed Range");
    char *a = getStr("Please enter the specified range [a; b] |||| [P.S.: char *]\nEnter first key:");
    //scanf("*c");
    char *b = getStr("Enter second key:");
    if (!a || !b) {
        puts("Error input, EOF inspected");
        return 0;
    }
    puts("Key output:");
    directTreeTraversalInTheRange(*proot, a, b);
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
        } else {
            info = atoi(buffer);
            insert(proot, key, info);
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
            if (insert(&root, k, rand()))
            	++i;
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i)
            if (deleteNode(root, key[i])) ++m;
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %lf\n", 10 - n, (10 - n) * cnt, (double)(last - first) / CLOCKS_PER_SEC);

    }
    delTree(&root);
    return 1;
}

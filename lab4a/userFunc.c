#include <stdio.h>

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
int D_Find(Node **proot)
{
    int k, n;
//    puts("Enter key: -->");
//    n = getInt(&k);
    char *key = getStr("Enter key [P.S.: char *]: -->");
    if (!key)
        return 0;
    //int len = findAll(masPtr, *proot, key);
    Node **masPtr = (Node **) calloc(50, sizeof(Node *));
    //masPtr = (Node **)calloc(i + 1, sizeof(Node *));
    int len = findAll(masPtr, *proot, key);
    masPtr = realloc(masPtr, len * sizeof(Node *));
    if (len >= 0) {
        for (size_t i = 0; i < len; ++i) {
            printf("key = %s\ninformation = %d\nWas found\n", masPtr[i]->key, masPtr[i]->info);
            puts("**************************");
        }
    }
    else
        printf("Node with key %s was not found\n", k);
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
    rc = delNode(proot, key);
    if (rc)
        printf("Ok\n");
    else
        printf("Node with key %d was not found\n", k);
    return OK;
}

// диалоговая функция вывода содержимого таблицы
int D_Show(struct Node **proot) {
    printf("\n=== Your tree: ===\n\n");
    putTree(*proot, 0);

    puts("\nDirect Tree Traversal");
    directTreeTraversal(*proot);
    return OK;
}


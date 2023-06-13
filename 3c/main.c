#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "hashTable.h"
#include "userFunc.h"
#include "checkFunc.h"

int main() {
    const char *msgs[] = {"0. Quit", "1. Add",
                          "2. Find", "3. Delete",
                          "4. Show", "5. CheckRelease",
                          "6. Import from file"};
    const int cntMsgs = sizeof(msgs) / sizeof(msgs[0]);

    Table *table = calloc(1, sizeof(Table)); //Сделать функция create_table

    int (*fptr[])(Table *) = {NULL, D_Add, D_Find,
                              D_Delete, D_Show,
                              D_CheckRelease, D_Import};
    int rc;

    if (D_Load(table) == 0) // Загружаем таблицу, подготавливаем к работе файл данных
        return 1;
    while (rc = dialog(msgs, cntMsgs)) {
        if (!fptr[rc](table)) {
            break;
        }
    }
    save(table); // Выгружаем таблицу в файлик и со спокойной душей идем пить чай
    printf("That's all. Bye!\n");
    delTable(table);
    return 0;
}

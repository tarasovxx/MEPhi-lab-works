#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "table.h"
#include "func.h"

int main() {
    const char *msgs[] = {"0. Quit", "1. Add",
                          "2. Find", "3. Delete",
                          "4. Show", "5. Reorganaze"};

    const int cntMsgs = sizeof(msgs) / sizeof(msgs[0]);

    Table *table = calloc(1, sizeof(Table)); //Сделать функция create_table

    int D_Add(Table *), // вставка элемента
    D_Find(Table *), // поиск элемента
    D_Delete(Table *), // удаление элемента
    D_Show(Table *), //Вывод
    D_Reorganization(Table *); // реорганизация таблицы (сборщик мусора)
    //D_Import(Table *);


    int (*fptr[])(Table *) = {NULL, D_Add, D_Find,
                               D_Delete, D_Show,
                               D_Reorganization}; //D_Import};
//    int m;
//    printf("Enter the msize value --->");
//    getInt(&m);
//    table->msize = m;

    //table->ks = (KeySpace *) calloc(table->msize, sizeof(KeySpace));
    int rc;

    if (D_Load(table) == 0) //загружаем таблицу, подготавливаем к работе файл данных
        return 1;

    while (rc = dialog(msgs, cntMsgs)) {
        if (!fptr[rc](table)) {
            break;
        }
    }

    save(table); // Выгружаем таблицу, закрываем файл данных
    printf("That's all. Bye!\n");
    delTable(table);
    return 0;
}

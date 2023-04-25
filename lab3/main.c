#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "table.h"
#include "func.h"

int main() {
    const char *msgs[] = {"0. Quit", "1. Add",
                          "2. Find", "3. Delete",
                          "4. Show", "5. Reorganaze",
                          "6. Import from file"};
    const int cntMsgs = sizeof(msgs) / sizeof(msgs[0]);

    Table *table = calloc(1, sizeof(Table)); //Сделать функция create_table

    int D_Add(Table *), // вставка элемента
    D_Find(Table *), // поиск элемента
    D_Delete(Table *), // удаление элемента
    D_Show(Table *), //Вывод
    D_Reorganization(Table *), // реорганизация таблицы (сборщик мусора)
    D_Import(Table *);


    int (*fptr[])(Table *) = {NULL, D_Add, D_Find,
                               D_Delete, D_Show,
                               D_Reorganization, D_Import};
    int m;
    printf("Enter the msize value --->");
    getInt(&m);
    table->msize = m;

    table->ks = (KeySpace *) calloc(table->msize, sizeof(KeySpace));
    //for (int i = 0; i < table->msize; ++i) {
    	//table->ks[i].data = calloc(1, sizeof(Item));
    	//table->ks[i].data->info = calloc(40, sizeof(char));
    //}
    int rc;
    while (rc = dialog(msgs, cntMsgs)) {
        if (!fptr[rc](table)) {
            break;
        }
    }
    printf("That's all. Bye!\n");
    delTable(table);
    return 0;
}

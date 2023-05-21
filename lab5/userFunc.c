#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userFunc.h"
#include "graph.h"
#include "checkFunc.h"
#include "error.h"

#define INDENT "-------------------------------------------------------------------------"

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
        puts("Make your choice---> ");

        n = getInt(&rc); // ввод номера альтернативы
        if (n == 0) // конец файла - конец работы
            rc = 0;
    } while (rc < 0 || rc >= N);

    return rc;
}

int D_AddVertex(Graph **graph) {
    int checker, x, y, t;
    cellType type;
    printf("Please enter the coordinates\nx--->");
    checker = getUnsignedInt(&x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("y--->");
    checker = getUnsignedInt(&y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    const char *selectType = "Please choose the type of this cell\n1 - Usual\n2 - labirint entry\n3 - labirint exit";
    puts(selectType);
    checker = getUnsignedInt(&t);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    while (t < 1 || t > 3) {
        switch (t) {
            case 1:
                type = USUAL;
                break;
            case 2:
                type = LABIRINT_ENTRY;
                break;
            case 3:
                type = LABIRINT_EXIT;
                break;
            default:
                puts("Incorrect value, please enter type again\n");
                puts(selectType);
        }
    }
    int r = addVertex(graph, x, y, type);
    if (r) {
        puts(INDENT);
        puts("The insertion was completed successfully");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    return OK;
}

int D_AddEdge(Graph **graph) {
    int checker, src, dest;
    printf("Enter the numbers of the vertices between which you want to add an edge\nsrc---> ");
    checker = getUnsignedInt(&src);
    if (checker == 0) return 0;
    printf("dest: ");
    checker = getUnsignedInt(&dest);
    if (checker == 0) return 0;
    int r = addEdge(graph, src, dest);
    if (r) {
        puts(INDENT);
        puts("The insertion was completed successfully");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    return OK;
}

int D_DeleteVertex(Graph **graph) {
    int checker, id;
    printf("Please enter the id of the vertex you want to delete\n id---> ");
    checker = getInt(&id);
    if (checker == 0) return 0;
    int r = deleteVertex(graph, id);
    if (r) {
        puts(INDENT);
        puts("Vertex successfully deleted");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    return OK;
}

int D_DeleteEdge(Graph **graph) {
    int checker, src, dest;
    printf("Enter the numbers of the vertices between which you want to add an edge\nsrc---> ");
    checker = getUnsignedInt(&src);
    if (checker == 0) return 0;
    printf("dest: ");
    checker = getUnsignedInt(&dest);
    if (checker == 0) return 0;
    int r = deleteEdge(graph, src, dest);
    if (r) {
        puts(INDENT);
        puts("Edge successfully removed");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    return OK;
}

int D_Update(Graph **graph) {
    int checker, id;
    printf("Please enter the id of the vertex you want to change\nid---> ");
    checker = getUnsignedInt(&id);
    int x, y, t;
    cellType type;
    printf("Please enter the coordinates\nx---> ");
    checker = getUnsignedInt(&x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("y---> ");
    checker = getUnsignedInt(&y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    puts("Please choose the type of this cell\n1 - Usual\n2 - labirint entry\n3 - labirint exit");
    checker = getUnsignedInt(&t);
    if (t == 0)
        return 0; // Обнаружен конец файла
    switch(t) {
        case 1:
            type = USUAL; break;
        case 2:
            type = LABIRINT_ENTRY; break;
        case 3:
            type = LABIRINT_EXIT; break;
        default:
            puts("Incorrect value");
    }
    int r = updateVertex(graph, id, x, y, type); //Возможно мы не изменяем граф, а просто делаем что-то внтури функции, подумай над этим
    if (r) {
        puts(INDENT);
        puts("The value has been updated successfully");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    return OK;
}

int D_Show(Graph **graph) {
    puts(INDENT);
    printGraph(*graph);
    puts(INDENT);
    return OK;
}

int D_Traversal(Graph **graph) { // Проверка достижимости хотя бы одного из выходов из указанной точки входа (поиск в глубину)
    // В данной функции мы просто проверям, можем ли мы каким-нибудь образом "выйти из лабиринта"
    // Для проверки используме алгоритм depth-first search
    int checker, entry, flag;
    printf("Enter the entrance point to the maze to check if it is possible to exit the maze\nLabirint entry --->");
    checker = getUnsignedInt(&entry);
    flag = checkReachability(graph, entry);
    if (flag) {
        puts(INDENT);
        printf("The output is achievable");
        puts(INDENT);
    }
    else {
        printf("The output is not achievable");
    }
}

int D_ShortestPath(Graph **graph) {
    int k;
    puts("");
}

int D_Modify(Graph **graph) {
    int k;
    puts("");
}

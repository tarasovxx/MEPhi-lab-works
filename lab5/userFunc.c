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
    Pair *p = calloc(1, sizeof(Pair)); p->x = x; p->y = y;
    const char *selectType = "Please choose the type of this cell\n1 - Usual\n2 - labirint entry\n3 - labirint exit";
    puts(selectType);
    do {
        checker = getUnsignedInt(&t);
        if (checker == 0)
            return 0; // Обнаружен конец файла
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
                break;
        }
    } while (t < 1 || t > 3);
    int r = addVertex(graph, &p, type);
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
    int checker, src_x, src_y, dest_x, dest_y;
    printf("Enter the coordinates of the vertex from which the edge will exit\nsrc->x---> ");
    checker = getUnsignedInt(&src_x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("src->y--->");
    checker = getUnsignedInt(&src_y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("Enter the coordinates of the vertex that the edge will enter\ndest->x---> ");
    checker = getUnsignedInt(&dest_x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("src->y--->");
    checker = getUnsignedInt(&dest_y);
    Pair *src = calloc(1, sizeof(Pair)); src->x = src_x; src->y = src_y;
    Pair *dest = calloc(1, sizeof(Pair)); dest->x = dest_x; dest->y = dest_y;
    int r = addEdge(graph, &src, &dest);
    if (r) {
        puts(INDENT);
        puts("The insertion was completed successfully");
        puts(INDENT);
        free(src);
    }
    else {
        puts("Oooops.... An error has occurred");
        free(src);
        free(dest);
    }
    //free(src);
    //free(dest);

    return OK;
}

int D_DeleteVertex(Graph **graph) {
    int checker, x, y;
    printf("Please enter the coordinates of the vertex you want to delete\n x---> ");
    checker = getUnsignedInt(&x);
    if (checker == 0) return 0;
    printf("y--->");
    checker = getUnsignedInt(&y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    Pair *p = calloc(1, sizeof(Pair)); p->x = x; p->y = y;
    int r = deleteVertex(graph, p);
    if (r) {
        puts(INDENT);
        puts("Vertex successfully deleted");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    free(p);
    return OK;
}

int D_DeleteEdge(Graph **graph) {
    int checker, src_x, src_y, dest_x, dest_y;
    printf("Enter the coordinates of the vertex from which the edge you want to remove comes out\nsrc->x---> ");
    checker = getUnsignedInt(&src_x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("src->y--->");
    checker = getUnsignedInt(&src_y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("Enter the coordinates of the vertex that contains the edge you want to delete\ndest->x---> ");
    checker = getUnsignedInt(&dest_x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("src->y--->");
    checker = getUnsignedInt(&dest_y);
    Pair *src = calloc(1, sizeof(Pair)); src->x = src_x; src->y = src_y;
    Pair *dest = calloc(1, sizeof(Pair)); dest->x = dest_x; dest->y = dest_y;
    int r = deleteEdge(graph, src, dest);
    if (r) {
        puts(INDENT);
        puts("Edge successfully removed");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    free(src);
    free(dest);
    return OK;
}

int D_Update(Graph **graph) {
    int checker, x, y, t;
    cellType type;
    printf("Please enter the coordinates\nx---> ");
    checker = getUnsignedInt(&x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("y---> ");
    checker = getUnsignedInt(&y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    const char *selectType = "Please choose the type of this cell\n1 - Usual\n2 - labirint entry\n3 - labirint exit";
    puts(selectType);
    //checker = getUnsignedInt(&t);
   	do {
		checker = getUnsignedInt(&t);
		if (checker == 0)
		    return 0; // Обнаружен конец файла
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
		        break;
		}
    } while (t < 1 || t > 3);
    Pair *p = calloc(1, sizeof(Pair)); p->x = x; p->y = y;
    int r = updateVertex(graph, p, type); //Возможно мы не изменяем граф, а просто делаем что-то внтури функции, подумай над этим
    if (r) {
        puts(INDENT);
        puts("The value has been updated successfully");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    free(p);
    return OK;
}

int D_Show(Graph **graph) {
	puts("0 - USUAL\n1 - ENTRY\n2 - EXIT");
    puts(INDENT);
    printGraph(*graph);
    puts(INDENT);
    return OK;
}

int D_Traversal(Graph **graph) { // Проверка достижимости хотя бы одного из выходов из указанной точки входа (поиск в глубину)
    // В данной функции мы просто проверям, можем ли мы каким-нибудь образом "выйти из лабиринта"
    // Для проверки используме алгоритм depth-first search
    int checker, entry_x, entry_y, flag;
    printf("Enter the coordinates of the entrance point to the maze to check if it is possible to exit the maze\nLabirint entry x--->");
    checker = getUnsignedInt(&entry_x);
    if (checker == 0) return 0;
    printf("Labirint entry y--->");
    checker = getUnsignedInt(&entry_y);
    if (checker == 0) return 0;
    Pair *entry = calloc(1, sizeof(Pair)); entry->x = entry_x; entry->y = entry_y;
    flag = checkReachability(*graph, entry);
    if (flag) {
        puts(INDENT);
        printf("The output is achievable\n");
        puts(INDENT);
    }
    else {
    	puts(INDENT);
        printf("The output is not achievable\n");
        puts(INDENT);
    }
    free(entry);
    return OK;
}

int D_ShortestPath(Graph **graph) {
    int checker, src_x, src_y, dest_x, dest_y;
    printf("Enter the coordinates of the entrance point to the maze\nsrc->x---> ");
    checker = getUnsignedInt(&src_x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("src->y--->");
    checker = getUnsignedInt(&src_y);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("Enter the coordinates of the exit point from the maze\ndest->x---> ");
    checker = getUnsignedInt(&dest_x);
    if (checker == 0)
        return 0; // Обнаружен конец файла
    printf("src->y--->");
    checker = getUnsignedInt(&dest_y);
    Pair *src = calloc(1, sizeof(Pair)); src->x = src_x; src->y = src_y;
    Pair *dest = calloc(1, sizeof(Pair)); dest->x = dest_x; dest->y = dest_y;
    Pair *path = calloc((*graph)->size, sizeof(Pair));
    int pathLength = shortestPath(*graph, src, dest, &path); //dijkstra

    if (pathLength) {
        puts(INDENT);
        // Делаем вывод пути
        for (int i = pathLength - 1; i >= 0; i--) {
            printf("(%u;%u)", path[i].x, path[i].y);
            if (i != 0) printf(" -> ");
            else puts("");
        }
        puts("The insertion was completed successfully\n");
        puts(INDENT);
    }
    else {
        puts("Oooops.... An error has occurred");
    }
    free(path);
    free(src);
    free(dest);
    return OK;
}

int D_Modify(Graph **graph) {
    int status = buildMST(*graph);
    if (status == 1) {
        puts(INDENT);
        printf("Entry or exit vertex not found.\n");
        puts(INDENT);
    }
    else {
        puts(INDENT);
        puts("The graph is modified to a minimal spanning tree");
        puts(INDENT);
    }


    return 1;
}

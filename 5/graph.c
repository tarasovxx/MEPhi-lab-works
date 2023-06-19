#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "graph.h"
#include "checkFunc.h"
#include "hashTable.h"

int addVertex(Graph **graph, Pair ** p, cellType type) {
    int ch = 0;
    if (findVertex(*graph, *p)) {
        updateVertex(graph, *p, type);
        free(*p);
        return 1;
    }
    if (!(*graph)) {
        *graph = calloc(1, sizeof(Graph));
        ch = 1;
    }
    Vertex *newVertex = calloc(1, sizeof(Vertex));
    //newVertex->coordinates = calloc(1, sizeof(Pair));
    newVertex->coordinates = *p;
    newVertex->type = type;
    newVertex->next = (*graph)->head;
    (*graph)->head = newVertex;
    (*graph)->size++;
    if (ch == 1) (*graph)->head->next = NULL;
    return 1;
}

int addEdge(Graph **graph, Pair **src, Pair **dest) {
    // Ищем вершину src среди Vertex
    // Если нашли, то проверяем соседняя ли это клетка
    Vertex *first = findVertex((*graph), *src);
    Vertex *second = findVertex((*graph), *dest);
    if (!first || !second) {
        // Одно или оба из заднных вершин отсутствуют
        return 0;
    }
    if (abs(first->coordinates->x - second->coordinates->x) <= 1 &&
            abs(first->coordinates->y - second->coordinates->y) <= 1) {
        // В этом случае добовляем ребро между вершинами, все условия пройдены
        Node *newNode = calloc(1, sizeof(Node));
        //newNode->dest = calloc(1, sizeof(Pair));
        newNode->dest = *dest;
        //newNode->dest->x = (*dest)->x;
        //newNode->dest->y = (*dest)->y;
        newNode->next = first->edge;
        first->edge = newNode;
    }
    else {
        // Существуют условия не позволяющие добавить ребро
        return 0;
    }
    return 1;
}

int pairCompare(Pair *first, Pair *second) {
    if (first->x == second->x && first->y == second->y) return 1;
    return 0;
}

Vertex *findVertex(Graph *graph, Pair *target) {
    if (!graph) return NULL;
    Vertex *ptr = graph->head;
    while (ptr) {
        if (pairCompare(target, ptr->coordinates)) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

int deleteVertex(Graph **graph, Pair *target) {
    if (!(*graph)) return 0;
    Vertex *cur = (*graph)->head;
    Vertex* prev = NULL;

    while (cur && !pairCompare(cur->coordinates, target)) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) return 1; //Нужная нам вершина не найдена
    Vertex *temp = (*graph)->head;
    while(temp) { // Проходочка по всем вершинам, и удалени е ребер, которые связаны с удаляемой вершиной
        Node *edge = temp->edge;
        Node *prevEdge = NULL;
        while (edge) {
            if (pairCompare(edge->dest, target)) {
                if (!prevEdge) temp->edge = edge->next;
                else prevEdge->next = edge->next;
                free(edge->dest);
                free(edge);
                break;
            }
            prevEdge = edge;
            edge = edge->next;
            free(prevEdge->dest);
            free(prevEdge);
            prevEdge = NULL;
        }
        temp = temp->next;
    }
    // Удаляем саму вершину
    if (!prev) {
        (*graph)->head = cur->next;
    }
    else {
        prev->next = cur->next;
    }
    free(cur->coordinates);
    free(cur);
    (*graph)->size--;
    return 1;
}

int deleteEdge(Graph **graph, Pair *src, Pair *dest) {
    if (!(*graph)) return 0;
    Vertex *cur = findVertex((*graph), src);
    if (!cur) return 0; //Не нашли вершинку
    Node *edge = cur->edge;
    Node *prevEdge = NULL;
    while (edge && !pairCompare(edge->dest, dest)) {
        prevEdge = edge;
        edge = edge->next;
    }
    if (!edge) return 0; // Не нашли ребро
    if (!prevEdge) {
        cur->edge = edge->next;
    }
    else {
        prevEdge->next = edge->next;
    }
    free(edge->dest);
    free(edge);
    return 1;
}

int updateVertex(Graph **graph, Pair *id, cellType type) {
    Vertex *target = findVertex((*graph), id);
    if (!target) return 0;
    target->type = type;
    // Не имеет смысла менять координаты, так как эта оперцаия равносильна удаление и добавление обратно

//    if (target->coordinates->x != x || target->coordinates->y != y) {
//        target->coordinates->x = x;
//        target->coordinates->y = y;
//        // Теперь пробегаемся по всем смежным ребрам вершины и проверяем выполнения условия в случе невыполения удаляем ребро
//        Node *edge = target->edge;
//        while (edge) {
//            Vertex *temp = findVertex((*graph), edge->dest);
//            if (abs(target->coordinates->x - temp->coordinates->x) <= 1 ||
//                abs(target->coordinates->y - temp->coordinates->y) <= 1) {
//                deleteEdge(graph, id, edge->dest);
//            }
//            edge = edge->next;
//        }
//    }

    return 1;
}

int printGraph(Graph *graph) {
    Vertex* ptr = graph->head;
    while (ptr) {
        cellType x = ptr->type;
        printf("%d(%u;%u)", x, ptr->coordinates->x, ptr->coordinates->y);
        Node *edge = ptr->edge;
        while (edge) {
            printf(" -> ");
            printf("(%u;%u)", edge->dest->x, edge->dest->y);
            edge = edge->next;
        }
        puts("");
        ptr = ptr->next;
    }
}

int delGraph(Graph **graph) {
    Vertex* ptr = (*graph)->head;
    Vertex* prevPtr = NULL;
    while (ptr) {
        free(ptr->coordinates);
        Node *edge = ptr->edge;
        Node *prevEdge = NULL;
        while (edge) {
            prevEdge = edge;
            edge = edge->next;
            free(prevEdge->dest);
            free(prevEdge);
        }
        prevPtr = ptr;
        ptr = ptr->next;
        free(prevPtr);
    }
    free(*graph);
}

int checkReachability(Graph *graph, Pair *entry) {
    Vertex *targetEntry = findVertex(graph, entry);
    if (targetEntry) {
    	Table *t = calloc(1, sizeof(Table));
        t->ks = calloc(graph->size, sizeof(KeySpace)); // уже всё заполнено нулями
        t->msize = graph->size;
    	int ans = dfs(graph, targetEntry, t);
    	tableDestroy(&t);
        return ans;
    }
    else {
    	return 0;
    }
}

int dfs(Graph *graph, Vertex *vertex, Table *t) {
    tableInsert(&t, vertex->coordinates, 1); // Помечаем 1, посещенная вершина
    int value = vertex->type;
    if (vertex->type == LABIRINT_EXIT)
        return 1;
    Node *adjacent = vertex->edge;
    while (adjacent) {
        if (!tableFind(t, adjacent->dest))
            if (dfs(graph, findVertex(graph, adjacent->dest), t))
                return 1;
        adjacent = adjacent->next;
    }
    return 0;
}

int shortestPath(Graph *graph, Pair *src, Pair *dest, Pair **path) {

	Vertex *start = findVertex(graph, src);
    Vertex *finish = findVertex(graph, dest);
    // Проверка на то, что start - вход, finish - выход
    if (!start || !finish) return 0;
    if (start->type != LABIRINT_ENTRY || finish->type != LABIRINT_EXIT) return 0;


    Table *parent = calloc(1, sizeof(Table));
    parent->ks = calloc(graph->size, sizeof(KeySpace));
    Table *visited = calloc(1, sizeof(Table));
    visited->ks = calloc(graph->size, sizeof(KeySpace));
    Table *distances = calloc(1, sizeof(Table));
    distances->ks = calloc(graph->size, sizeof(KeySpace));
    Vertex *vertex = graph->head;
    parent->msize = distances->msize = visited->msize = graph->size;
    // Инициализация массивов
    while (vertex) {
        tableInsert(&distances, vertex->coordinates, INT_MAX);
        tableInsert(&parent, vertex->coordinates, -1);
        tableInsert(&visited, vertex->coordinates, 0);
        vertex = vertex->next;
        // Всё остальное уже покрыто, так как используется calloc (покрыто - заполенено нулями)
    }
    tableInsert(&distances, start->coordinates, 0);

    // поиск кратчайшего пути
    for (Vertex *cur = graph->head; cur != NULL; cur = cur->next) {
        int minDist = INT_MAX;
        Pair *rememberPair = NULL;
        // Найти вершину с минимальным расстоянием
        for (Vertex *minim = graph->head; minim != NULL; minim = minim->next) {
            KeySpace *tempDist = tableFind(distances, minim->coordinates);
            KeySpace *tempVisit = tableFind(visited, minim->coordinates);
            if (tempVisit->info == 0 && tempDist->info < minDist) {
                minDist = tempDist->info;
                rememberPair = minim->coordinates;
            }
        }
        Node *currentEdge = NULL;
        if (rememberPair) {
            tableInsert(&visited, rememberPair, 1); // Пометили вершину как посещённую
            // Нужно обновить расстояния до смежных вершин
            Vertex *currentVertex = findVertex(graph, rememberPair);
            currentEdge = currentVertex->edge;
        }
        while (currentEdge) {
            Pair *destingth = currentEdge->dest;
            KeySpace *tempDist = tableFind(distances, destingth);
            KeySpace *tempVisit = tableFind(visited, destingth);
            if (tempVisit->info == 0 && minDist + 1 < tempDist->info) {
                //tableDelete(distances, destingth);
                tableInsert(&distances, destingth, minDist + 1);
                tableInsertWithParent(&parent, destingth, rememberPair); // В пару dest мы попали из пары rememberPair
            }
            currentEdge = currentEdge->next;
        }
    }
    // Восстановление пути
    KeySpace *tempDist = tableFind(distances, finish->coordinates);
    if (tempDist->info == INT_MAX) {
        printf("The path does not exist");
    }
    else {
        int len = 0;
        Pair *curr = finish->coordinates;
        KeySpace *par = tableFind(parent, curr);
        while (par->info != -1) {
            (*path)[len++] = *curr;
            par = tableFind(parent, curr);
            curr = par->parent;
        }
        
        tableDestroy(&parent);
        tableDestroy(&distances);
        tableDestroy(&visited);
        return len;
    }
    
    tableDestroy(&parent);
    tableDestroy(&distances);
    tableDestroy(&visited);
    return 0;
}

void buildMinimumSpanningTree(Graph *graph) {
    Graph *mst = calloc(1, sizeof(Graph));
    mst->size = graph->size;

    // Добавляем все вершины из исходного графа
    Vertex *vertex = graph->head;
    while (vertex != NULL) {
        addVertex(&mst, &vertex->coordinates, USUAL);
        vertex = vertex->next;
    }


}


Vertex* findEntryVertex(Graph* graph) {
    Vertex* currentVertex = graph->head;

    while (currentVertex != NULL) {
        if (currentVertex->type == LABIRINT_ENTRY) {
            return currentVertex;
        }
        currentVertex = currentVertex->next;
    }

    return NULL; // Входной вершину не найдена
}

Vertex* findExitVertex(Graph* graph) {
    Vertex* currentVertex = graph->head;

    while (currentVertex != NULL) {
        if (currentVertex->type == LABIRINT_EXIT) {
            return currentVertex;
        }
        currentVertex = currentVertex->next;
    }

    return NULL; // Выходная вершина не найдена
}

int buildMST(Graph *graph) { // Так как нам нужно модифицировать, убдем всё делать на месте // Алгоритм Прима
    Vertex *startVertex = findEntryVertex(graph);
    Vertex *exitVertex = findExitVertex(graph);
    if (startVertex == NULL || exitVertex == NULL) {
        return 1;
    }
    // Помечаем все вершины как непосещенные
    Vertex *currentVertex = graph->head;
    Table *key = calloc(1, sizeof(Table));
    key->ks = calloc(graph->size, sizeof(KeySpace));
    Table *mstSet = calloc(1, sizeof(Table));
    mstSet->ks = calloc(graph->size, sizeof(KeySpace));
    Table *par = calloc(1, sizeof(Table));
    par->ks = calloc(graph->size, sizeof(KeySpace));
    key->msize = par->msize = mstSet->msize = graph->size;
    while (currentVertex != NULL) {
        tableInsert(&key, currentVertex->coordinates, INT_MAX);
        tableInsert(&mstSet, currentVertex->coordinates, 0);
        tableInsert(&par, currentVertex->coordinates, -1);
        currentVertex = currentVertex->next;
    }
    tableInsert(&key, startVertex->coordinates, 0);
    for (size_t i = 0; i < graph->size; ++i) {
        // Extract_Min // Ну якобы пикаем минимальную, хотя у нас нет весов на рёбрах ????
        int min = INT_MAX;
        Vertex* minVertex = NULL;
        for (Vertex *v = graph->head; v; v = v->next) {
            KeySpace *tempSet = tableFind(mstSet, v->coordinates);
            KeySpace *tempKey = tableFind(key, v->coordinates);
            if (tempSet->info == 0 && tempKey->info < min) {
                min = tempKey->info;
                minVertex = v;
            }
        }
        Node *ptrEdge = NULL;
        if (minVertex) {
            tableInsert(&mstSet, minVertex->coordinates, 1);
            ptrEdge = minVertex->edge;
        }
        while (ptrEdge != NULL) {
            Pair* temp = ptrEdge->dest;
            KeySpace *tempSet = tableFind(mstSet, temp);
            KeySpace *tempKey = tableFind(key, temp);
            if (tempSet->info == 0 && tempKey->info == INT_MAX) {
                tableInsert(&key, temp, 0);
                tableInsertWithParent(&par, temp, minVertex->coordinates);
            }
            ptrEdge = ptrEdge->next;
        }
    }
    for (Vertex *v = graph->head; v; v = v->next)  {
    	if (!pairCompare(v->coordinates, exitVertex->coordinates) && !pairCompare(v->coordinates, startVertex->coordinates)) updateVertex(&graph, v->coordinates, USUAL);
        KeySpace *tempPar = tableFind(par, v->coordinates);
        if (tempPar->parent) {
        	int sizeNode = 0;
            Vertex *isk = findVertex(graph, tempPar->parent);
            Node *ptr = isk->edge;
            while (ptr) {
                ++sizeNode;
                ptr = ptr->next;
            }
            //if (!pairCompare(v->coordinates, exitVertex->coordinates)) updateVertex(&graph, v->coordinates, USUAL); // Все вершины, которые не является ни start ни exit, превращаются в USUAL, либо пможно удалять
            if (sizeNode > 1 && !pairCompare(v->coordinates, exitVertex->coordinates) && !pairCompare(v->coordinates, startVertex->coordinates) && !pairCompare(tempPar->parent, startVertex->coordinates)) {
                deleteEdge(&graph, v->coordinates, tempPar->parent);
                deleteEdge(&graph, tempPar->parent, v->coordinates);
            }
        }
    }
    tableDestroy(&par);
    tableDestroy(&key);
    tableDestroy(&mstSet);
    return 0;
}

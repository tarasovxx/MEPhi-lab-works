#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "checkFunc.h"

int addVertex(Graph **graph, unsigned int x, unsigned int y, cellType type) {
    int id = 1;
    Vertex *newVertex = calloc(1, sizeof(Vertex));
    newVertex->coordinates = calloc(1, sizeof(Pair));
    newVertex->coordinates->x = x;
    newVertex->coordinates->y = y;
    newVertex->type = type;
    if (!(*graph)) {
        *graph = calloc(1, sizeof(graph));
        id = 1;
    }
    else id = (*graph)->head->index + 1;
    newVertex->index = id;
    newVertex->next = (*graph)->head;
    (*graph)->head = newVertex;
    (*graph)->size++;
    return 1;
}

int addEdge(Graph **graph, int src, int dest) {
    // Ищем вершину src среди Vertex
    // Если нашли, то проверяем соседняя ли это клетка
    Vertex *first = findVertex((*graph), src);
    Vertex *second = findVertex((*graph), dest);
    if (!first || !second) {
        // Одно или оба из заднных вершин отсутствуют
        return 0;
    }
    if (abs(first->coordinates->x - second->coordinates->x) <= 1 ||
            abs(first->coordinates->y - second->coordinates->y) <= 1) {
        // В этом случае добовляем ребро между вершинами, все условия пройдены
        Node *newNode = calloc(1, sizeof(Node));
        newNode->dest = dest;
        newNode->next = first->edge;
        first->edge = newNode;
    }
    else {
        // Существуют условия не позволяющие добавить ребро
        return 0;
    }
    return 1;
}

Vertex *findVertex(Graph *graph, int index) {
    Vertex *ptr = graph->head;
    while (ptr) {
        if (index == ptr->index) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

int deleteVertex(Graph **graph, int index) {
    if (!(*graph)) return 0;
    Vertex *cur = (*graph)->head;
    Vertex* prev = NULL;

    while (cur && cur->index != index) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) return 1; //Нужная нам вершина не найдена
    Vertex *temp = (*graph)->head;
    while(temp) { // Проходочка по всем вершинам, и удалени е ребер, которые связаны с удаляемой вершиной
        Node *edge = temp->edge;
        Node *prevEdge = NULL;
        while (edge) {
            if (edge->dest == index) {
                if (!prevEdge) temp->edge = edge->next;
                else prevEdge->next = edge->next;
                free(edge);
                break;
            }
            prevEdge = edge;
            edge = edge->next;
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

int deleteEdge(Graph **graph, int src, int dest) {
    if (!(*graph)) return 0;
    Vertex *cur = findVertex((*graph), src);
    if (!cur) return 0; //Не нашли вершинку
    Node *edge = cur->edge;
    Node *prevEdge = NULL;
    while (edge && edge->dest != dest) {
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
    free(edge);
    return 1;
}

int updateVertex(Graph **graph, int id, unsigned int x, unsigned int y, cellType type) {
    Vertex *target = findVertex((*graph), id);
    if (!target) return 0;
    target->type = type;
    if (target->coordinates->x != x || target->coordinates->y != y) {
        target->coordinates->x = x;
        target->coordinates->y = y;
        // Теперь пробегаемся по всем смежным ребрам вершины и проверяем выполнения условия в случе невыполения удаляем ребро
        Node *edge = target->edge;
        while (edge) {
            Vertex *temp = findVertex((*graph), edge->dest);
            if (abs(target->coordinates->x - temp->coordinates->x) <= 1 ||
                abs(target->coordinates->y - temp->coordinates->y) <= 1) {
                deleteEdge(graph, id, edge->dest);
            }
            edge = edge->next;
        }
    }

    return 1;
}

int printGraph(Graph *graph) {
    Vertex* ptr = graph->head;
    while (ptr) {
        printf("VERTEX: %d[%u, %u]\tEDGE:", ptr->index, ptr->coordinates->x, ptr->coordinates->y);
        Node *edge = ptr->edge;
        while (edge) {
            printf("(%d ---> %d)\t", ptr->index, edge->dest);
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
            free(prevEdge);
        }
        puts("");
        prevPtr = ptr;
        ptr = ptr->next;
        free(prevPtr);
    }
}

int checkReachability(Graph *graph, int entry) {

}



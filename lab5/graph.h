#ifndef LAB5_GRAPH_H
#define LAB5_GRAPH_H

//// Ориентированный граф

//Типы клеток
typedef enum cellType{
    USUAL,
    LABIRINT_ENTRY,
    LABIRINT_EXIT
} cellType;

// Структура данных для хранения узлов списка смежности Graph
typedef struct Edge {
    int src, dest;
} Edge;

typedef struct Pair {
    unsigned int x;
    unsigned int y;
} Pair;

typedef struct Node {
    int dest;
    struct Node *next;
} Node;

typedef struct Vertex {
    unsigned int index;
    Pair *coordinates;
    cellType type;
    Node *edge;
    struct Vertex *next;
} Vertex;


typedef struct Graph { //// Список списка смежности :)
    int size;
    Vertex *head; // Список ребер смежных с данной вершиной
} Graph;

int addVertex(Graph **, unsigned int , unsigned int , cellType );

Vertex *findVertex(Graph *, int );

int addEdge(Graph **, int , int );

int deleteVertex(Graph **, int );

int deleteEdge(Graph **, int , int );

int updateVertex(Graph **, int , unsigned int , unsigned int , cellType );

int printGraph(Graph *);

int delGraph(Graph **);


#endif //LAB5_GRAPH_H


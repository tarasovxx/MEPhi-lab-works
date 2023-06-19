#ifndef LAB5_GRAPH_H
#define LAB5_GRAPH_H

//#include "func.h"
#include "hashTable.h"

//// Ориентированный граф
typedef struct Node {
    Pair *dest;
    struct Node *next;
} Node;

typedef struct Vertex {
    //unsigned int index;
    Pair *coordinates;
    cellType type;
    Node *edge;
    struct Vertex *next;
} Vertex;


typedef struct Graph { //// Список списка смежности :)
    int size;
    Vertex *head; // Список ребер смежных с данной вершиной
} Graph;

int addVertex(Graph **, Pair ** , cellType );

Vertex *findVertex(Graph *, Pair *);

int addEdge(Graph **, Pair **, Pair **);

int deleteVertex(Graph **, Pair *);

int deleteEdge(Graph **, Pair *, Pair *);

int updateVertex(Graph **, Pair *, cellType );

int printGraph(Graph *);

int delGraph(Graph **);

int pairCompare(Pair *, Pair *);

int checkReachability(Graph *, Pair *);

int dfs(Graph *, Vertex *, Table *);

int shortestPath(Graph *, Pair *, Pair *, Pair **);

Vertex* findExitVertex(Graph * );

Vertex* findEntryVertex(Graph *);

int buildMST(Graph *);

void visualizeGraph(Graph *);


#endif //LAB5_GRAPH_H

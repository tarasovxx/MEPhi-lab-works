#ifndef LAB5_USERFUNC_H
#define LAB5_USERFUNC_H

#include "graph.h"

int dialog(const char *[], int );

int D_AddVertex(Graph **);

int D_AddEdge(Graph **);

int D_DeleteVertex(Graph **);

int D_DeleteEdge(Graph **);

int D_Update(Graph **);

int D_Show(Graph **);

int D_Traversal(Graph **);

int D_ShortestPath(Graph **);

int D_Modify(Graph **);

#endif //LAB5_USERFUNC_H

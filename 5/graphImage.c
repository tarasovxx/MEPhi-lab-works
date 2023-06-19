#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

#include "graphImage.h"
#include "graph.h"

void visualizeGraph(Graph *graph) {
    FILE *dotFile = fopen("graph.dot", "w");
    if (dotFile == NULL) {
        printf("Error opening graph.dot file.\n");
        return;
    }

    fprintf(dotFile, "digraph G {\n");
    fprintf(dotFile, "node [shape=oval];\n");

    // Traverse vertices
    Vertex *vertex = graph->head;    
    while (vertex != NULL) {
        Pair *coordinates = vertex->coordinates;
        char label[50];
        char color[15];
        char shape[15];
        sprintf(label, "%u %u", coordinates->x, coordinates->y);
        switch (vertex->type) {
            case USUAL:
                strcpy(color, "black");
                break;
            case LABIRINT_ENTRY:
            	strcpy(shape, "square");
                strcpy(color, "green");
                break;
            case LABIRINT_EXIT:
            	strcpy(shape, "triangle");
                strcpy(color, "red");
                break;
        }

        //fprintf(dotFile, "%s [label=\"%s\", style=filled, fillcolor=%s];\n", label, label, color);
        Node *edge = vertex->edge;
        while (edge != NULL) {
            Pair *dest = edge->dest;
            char destLabel[50];
            char label[50];
            sprintf(label, "\"%u;%u\"", vertex->coordinates->x, vertex->coordinates->y);
            sprintf(destLabel, "\"%u;%u\"", dest->x, dest->y);
            fprintf(dotFile, "%s -> %s [color=%s];\n", label, destLabel, color);
            edge = edge->next;
        }
        vertex = vertex->next;
    }
    fprintf(dotFile, "}\n");
    fclose(dotFile);
}

void generateGraphImage() {
    system("dot -Tpng -o graph.png graph.dot");
}

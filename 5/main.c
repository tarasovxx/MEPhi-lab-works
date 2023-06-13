#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "userFunc.h"
#include "checkFunc.h"
#include "graphImage.h"

const char *msgs[] = {"0. Quit", "1. Add Vertex", "2. Add Edge", "3. Delete Vertex", "4. Delete Edge", "5. Update data[Vertex]", "6. Show", "7. DFS", "8. Shortest path", "9. Modification. Minimum spanning tree"};
const char NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*fptr[])(Graph **) = {NULL, D_AddVertex, D_AddEdge, D_DeleteVertex, D_DeleteEdge, D_Update, D_Show, D_Traversal, D_ShortestPath, D_Modify};

int main() {
    Graph *graph = NULL;
    int rc;
    while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](&graph))
            break;
    puts("That's all. Bye!\n");
    if (graph) {
    	visualizeGraph(graph);
    	generateGraphImage();
   	 	delGraph(&graph);
   	}
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

#include "graphvizDopTask.h"
#include "B_Tree.h"

// Function to recursively create graph nodes and edges
void createGraphNodes(Agraph_t *graph, Agnode_t *parentNode, Node *currentNode) {
    if (currentNode == NULL)
        return;

    char *keyInGraph = NULL;
    if (currentNode->key1) keyInGraph = strdup(currentNode->key1->data);
    if (currentNode->key1 != NULL && currentNode->key2 != NULL){
    	int curLen = strlen(keyInGraph), secondLen = strlen(currentNode->key2->data);
    	int len = curLen + secondLen + 2;
    	keyInGraph = realloc(keyInGraph, len * sizeof(char));
    	int k = 0;
    	for (size_t i = curLen; i < len; ++i) {
    		if (i == curLen) keyInGraph[i] = '|';
    		else if (k < secondLen) keyInGraph[i] = currentNode->key2->data[k++];
    		else keyInGraph[i] = '\0';
    		
    	}
    	
    }
    	//strcat(keyInGraph, currentNode->key2->data);
    Agnode_t*currentNodeGraph = agnode(graph, keyInGraph, 1);
    if (parentNode != NULL) {
        Agedge_t *edge = agedge(graph, parentNode, currentNodeGraph, NULL, 1);
        agsafeset(edge, "label", "", "");
    }
    free(keyInGraph);
    createGraphNodes(graph, currentNodeGraph, currentNode->left);
    createGraphNodes(graph, currentNodeGraph, currentNode->middle);
    createGraphNodes(graph, currentNodeGraph, currentNode->right);
}

// Function to visualize the tree using Graphviz
void visualizeTree(Node *root) {
    // Create a new directed graph
    Agraph_t *graph = agopen("G", Agdirected, NULL);
    
    // Create graph nodes and edges recursively
    createGraphNodes(graph, NULL, root);
    
    // Create the output file
    FILE *file = fopen("tree.dot", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.");
        agclose(graph);
        return;
    }
    
    // Write the graph to the file in DOT format
    agwrite(graph, file);
    
    // Close the file and the graph
    fclose(file);
    agclose(graph);
}

void generateTreeImage() {
    system("dot -Tpng -o tree.png tree.dot");
}

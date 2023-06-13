#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

#include "graphvizDopTask.h"
#include "binTree.h"

void generateDotFile(Node* root) {
	addThreadInOrder(root);
    FILE* file = fopen("tree.dot", "w");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        return;
    }

    Agraph_t* graph = agopen("tree", Agdirected, NULL); // Напрявленный граф

    // Рекурсивно добавляем узлы в граф
    while (root) {
    	if (root == NULL) return ; 
        Agnode_t* graphNode = agnode(graph, root->key, 1);

        if (root->left != NULL) {
            Agnode_t* leftNode = agnode(graph, root->left->key, 1);
            agedge(graph, graphNode, leftNode, NULL, 1);
            agedge(graph, leftNode, graphNode, NULL, 1);
        }

        if (root->right != NULL) {
            Agnode_t* rightNode = agnode(graph, root->right->key, 1);
            agedge(graph, graphNode, rightNode, NULL, 1);
            agedge(graph, rightNode, graphNode, NULL, 1);
        }
        if (root->left) {
            root = root->left;
            continue;
        }
        else if (root->right) {
            root = root->right;
            continue;
        }
        if (root->thread) {
        	Agnode_t *threadNode = agnode(graph, root->thread->key, 1);
        	agedge(graph, graphNode, threadNode, NULL, 1);
            root = root->thread;
            continue;
        }
        root = root->left;
    }

    agwrite(graph, file);
    fclose(file);
    agclose(graph);
    //agfree(graph, agnameof(graph));
    //free(graph);
}

void generateTreeImage() {
    system("dot -Tpng -o tree.png tree.dot");
}

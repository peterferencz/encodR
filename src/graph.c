#include "graph.h"

int graph_countLeaves(Node *root){
    if(root == NULL){
        return 0;
    }else if(root->left_0 == NULL && root->right_1 == NULL){
        return 1;
    }
    
    return graph_countLeaves(root->left_0) + graph_countLeaves(root->right_1);
}

#ifndef INCL_GRAPH
#define INCL_GRAPH

#include "codeword.c"

typedef struct Node{
    char codeword;

    struct Node *left_0;
    struct Node *right_1;
} Node;

int graph_countLeaves(Node *root){
    if(root == NULL){
        return 0;
    }else if(root->left_0 == NULL && root->right_1 == NULL){
        return 1;
    }
    
    return graph_countLeaves(root->left_0) + graph_countLeaves(root->right_1);
}

#endif
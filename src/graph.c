#include "graph.h"

// ================================= Functions =================================

/// @brief Megszámolja egy bináris fagráf leveleinek számát
/// @param root A fagráf gyökere
/// @return A fagráf leveleinek száma
int graph_countLeaves(Node *root){
    if(root == NULL){
        return 0;
    }else if(root->left_0 == NULL && root->right_1 == NULL){
        return 1;
    }
    
    return graph_countLeaves(root->left_0) + graph_countLeaves(root->right_1);
}

/// @brief Rekurzívan felszabadít egy fagráfot
/// @param root a felszabadítandó gráf gyökere
void freeTree(Node *root){
    if(root == NULL) { return; }
    freeTree(root->left_0);
    freeTree(root->right_1);
    free(root);
}
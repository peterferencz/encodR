#ifndef INCL_GRAPH
#define INCL_GRAPH

#include "codeword.c"

typedef struct Node{
    codeWord codeword;

    struct Node *left_0;
    struct Node *right_1;
} Node;

#endif
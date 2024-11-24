#pragma once

// ================================== Headers ==================================

#include <stdlib.h>
#include <stdbool.h>

#include "codeword.h"
#include "debug/debug.h"

// ================================== Structs ==================================

typedef struct Node{
    uchar codeword;
    bool set;

    struct Node *left_0;
    struct Node *right_1;
} Node;

// ============================ Function prototypes ============================

int graph_countLeaves(Node *root);
void freeTree(Node *root);

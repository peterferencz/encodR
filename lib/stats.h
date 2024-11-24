#pragma once

// ================================== Headers ==================================

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "graph.h"
#include "debug/debug.h"
#include "bin.h"

// ============================ Function prototypes ============================

void stats_printCompression(const char *src, const char *dest);

void stats_printCodetableTree(Node *root);
void stats_printCodetableArray(CodeWord array[], int elements);

void stats_printCodetableStatsArray(CodeWord array[], int elements);

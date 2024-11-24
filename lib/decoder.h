#pragma once

// ================================== Headers ==================================

#include <stdio.h>
#include <stdbool.h>


#include "debug/debug.h"
#include "main.h"
#include "bin.h"
#include "fileBuffer.h"
#include "codeword.h"
#include "graph.h"

// ============================ Function prototypes ============================

int decode(commandLineArguments args);

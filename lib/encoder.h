#ifndef INCL_ENCODER
#define INCL_ENCODER

// ================================== Headers ==================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "fileBuffer.h"
#include "codeword.h"
#include "bin.h"

#include "debug/debug.h"

// ================================== Structs ==================================

typedef struct codewordFrequency{
    float freq;
    CodeWord codeWord;
} codewordFrequency;

// ============================ Function prototypes ============================

int encode(commandLineArguments args);

#endif
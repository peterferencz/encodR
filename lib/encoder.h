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

/// @brief Kódolt szót és annak a szövegben előfordulásának gyakoriságát
/// eltároló struktúra
typedef struct codewordFrequency{
    /// @brief Az adott kódolt szó előfordulásának frekvenciája
    float freq;
    /// @brief Kódolt szó
    CodeWord codeWord;
} codewordFrequency;

// ============================ Function prototypes ============================

int encode(commandLineArguments args);

#endif
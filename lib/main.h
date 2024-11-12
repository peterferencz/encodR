#ifndef INCL_MAIN
#define INCL_MAIN
// ================================== Headers ==================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "debug/debug.h"

// ================================== Structs ==================================

/// @brief A program parancssori argumentumait rendező struktúra.
typedef struct commandLineArguments {
    /// @brief A --bemenet kapcsoló által megadott stream.
    FILE* infile;
    /// @brief A --kimenet kapcsoló által megadott stream.
    FILE* outfile;
    /// @brief Megajda, hogy a program kiírja-e a kódtáblát.
    bool displayTable;
    /// @brief Megajda, hogy a program kiírjon-e további számitásokat a program
    /// hatékonyságára vonatkozólag.
    bool displayStatistics;
} commandLineArguments;

#include "encoder.h"
#include "decoder.h"

/// @brief Megadja, hogy a program a 'kodol' vagy 'dekodol' paraméterrel lett
/// meghívva
enum MODE {
    ENCODE = 0,
    DECODE = 1,
    UNSET = -1
};

#endif
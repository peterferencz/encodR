#ifndef INCL_MAIN
#define INCL_MAIN

// ================================== Headers ==================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "debug/debug.h"
#include "stats.h"

// ================================== Structs ==================================

/// @brief Megadja, hogy a program a 'kodol' vagy 'dekodol' paraméterrel lett
/// meghívva
enum MODE {
    ENCODE = 0,
    DECODE = 1,
    UNSET = -1
};

/// @brief A program parancssori argumentumait rendező struktúra.
typedef struct commandLineArguments {
    /// @brief A --bemenet kapcsoló által megadott stream.
    const char* infile;
    /// @brief A --kimenet kapcsoló által megadott stream.
    const char* outfile;

    /// @brief A program üzemmódja: kódolás vagy dekódolás
    enum MODE mode;

    /// @brief Megajda, hogy a program kiírja-e a kódtáblát.
    bool displayTable;
    /// @brief Megajda, hogy a program kiírjon-e további számitásokat a program
    /// hatékonyságára vonatkozólag.
    bool displayStatistics;
} commandLineArguments;

#include "encoder.h"
#include "decoder.h"



#endif
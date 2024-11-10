#ifndef INCL_CLA
#define INCL_CLA

#include <stdio.h>
#include <stdbool.h>

/// @brief A program parancssori argumentumait rendező struktúra
typedef struct commandLineArguments {
    FILE* infile;
    FILE* outfile;
    bool displayTable;
    bool displayStatistics;
} commandLineArguments;

#endif

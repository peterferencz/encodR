#ifndef INCL_CLA
#define INCL_CLA

#include <stdio.h>
#include <stdbool.h>

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

#endif

#ifndef INCL_BUFFER
#define INCL_BUFFER

// ================================== Headers ==================================

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "debug/debug.h"
#include "bin.h"
#include "codeword.h"

// ================================== Structs ==================================

/// @brief Struktúra, mely lehetővé teszi a bitenkénti olvasást egy fájlból.
typedef struct InputFileBuffer {
    /// @brief A fájl, melyből olvasunk
    FILE *file;

    /// @brief Megadja, hogy az adott fájl olvasásánál hanyadik bitnél tartunk.
    /// Értéke 0 és 7 közötti.
    uchar *currentBit;
} InputFileBuffer;

/// @brief Struktúra, mely lehetővé teszi a bitenkénti írást egy fájlba.
typedef struct OutputFileBuffer {
    /// @brief A fájl, melybe írunk
    FILE *file;

    /// @brief A még nem a fájlba beírt bitek
    Bits *bits;
} OutputFileBuffer;

// ============================ Function prototypes ============================

InputFileBuffer buff_createInputFileBuffer(const char *path);
OutputFileBuffer buff_createOutputFileBuffer(const char *path);

void buff_destroyInputFileBuffer(InputFileBuffer buffer);
void buff_destroyOutputFileBuffer(OutputFileBuffer buffer);

void buff_rewind(InputFileBuffer buffer);

bool buff_writeBits(OutputFileBuffer buff, Bits bit);
bool buff_writeBit(OutputFileBuffer buff, Bits bit);
bool buff_writeChar(OutputFileBuffer buff, uchar val);
bool buff_writeInt(OutputFileBuffer buff, int val);
bool buff_flush(OutputFileBuffer buff);

Bits buff_readBit(InputFileBuffer buff);
Bits buff_readBits(InputFileBuffer buff, int bitCount);
Bits buff_readChar(InputFileBuffer buff);
Bits buff_readInt(InputFileBuffer buff);

#endif
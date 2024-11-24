#pragma once

// ================================== Headers ==================================

#include <stdbool.h>

#include "bin.h"

// ================================== Structs ==================================

/// @brief Előjel nélküli 8 bites karakter
typedef unsigned char uchar;

/// @brief Karakter, és az azt kódoló bitsorozat
typedef struct CodeWord{

    /// @brief Egy byte, melyet a Shanon-Fano kódolás szerint kódolunk
    uchar codeWord;

    /// @brief Egy bitsorozat, melyet a Shanon-Fano kódolás szerint a
    /// \a codeWord kódolt változata
    Bits bits;
} CodeWord;

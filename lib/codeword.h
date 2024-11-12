#ifndef INCL_CODEWORD
#define INCL_CODEWORD

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/// @brief Előjel nélküli 8 bites karakter
typedef unsigned char uchar;

/// @brief Tetszőleges hosszú bitsorozat eltárolására alkalmas struktúra
typedef struct Bits {
    /// @brief A tárolt szám
    /// A bitek jobbról balra értelmezendőek
    long long unsigned int b;

    /// @brief A tárolt bitsorozat hossza
    size_t length;
} Bits;

/// @brief Karakter, és az azt kódoló bitsorozat
typedef struct CodeWord{

    /// @brief Egy byte, melyet a Shanon-Fano kódolás szerint kódolunk
    uchar codeWord;

    /// @brief Egy bitsorozat, melyet a Shanon-Fano kódolás szerint a @ref{ codeWord } kódolt változata
    Bits bits;
} CodeWord;

/// @brief Hibás kimenetet jelentő bitsorozat, melynek hossza 0
#define NULLBIT ((Bits){ .b = 0, .length = 0 })

bool bits_equ(Bits b1, Bits b2);
bool isNullbit(Bits b);

#endif
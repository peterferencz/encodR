#ifndef INCL_BIN
#define INCL_BIN

// ================================== Headers ==================================

#include <stdio.h>
#include <stdbool.h>

// ================================== Structs ==================================

/// @brief Tetszőleges hosszú bitsorozat eltárolására alkalmas struktúra
typedef struct Bits {
    /// @brief A tárolt szám
    /// A bitek jobbról balra értelmezendőek
    long long unsigned int b;

    /// @brief A tárolt bitsorozat hossza
    size_t length;
} Bits;

/// @brief Hibás kimenetet jelentő bitsorozat, melynek hossza 0
#define NULLBIT ((Bits){ .b = 0, .length = 0 })

// ============================ Function prototypes ============================

Bits getBitFromRight(Bits bits, int n);

void bits_pushBit(Bits *bits, char b);
void bits_pushBits(Bits *bits, Bits append);

Bits bits_popBit(Bits *bits);
Bits bits_popBits(Bits *bits, int length);

void bits_cpy(Bits src, Bits *dest);

void bits_print(Bits bits);

bool bits_equ(Bits b1, Bits b2);
bool bits_isNullbit(Bits b);


#endif
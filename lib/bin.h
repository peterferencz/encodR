#ifndef INCL_BIN
#define INCL_BIN

#include <stdio.h>
#include "codeword.h"

Bits getBitFromRight(Bits bits, int n);

void bits_pushBit(Bits *bits, Bits append);

void print_bits(Bits bits);


#endif
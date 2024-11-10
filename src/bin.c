#ifndef INCL_BIN
#define INCL_BIN

#include <stdio.h>
#include "codeword.c"

/// @brief gets the nth character from the right of a given binary
/// @param bin 
/// @param length in bits
/// @param n 
/// @return 
//TODO why int?, is this not from the left?
char getBitFromRight(Bits bits, int n){
    return (bits.b >> (bits.length - 1 - n)) & 1;
}



#endif
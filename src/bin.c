#ifndef INCL_BIN
#define INCL_BIN

/// @brief gets the nth character from the right of a given binary
/// @param bin 
/// @param length in bits
/// @param n 
/// @return 
int getBitFromRight(int bin, int length, int n){
    return ((bin >> (length - 1 - n)) & 1);
}

#endif
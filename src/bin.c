#ifndef INCL_BIN
#define INCL_BIN

#include <stdio.h>
#include "codeword.c"


/// @brief Adott bitsorozatnak megadja a jobról számított \p n -edik bitjét
/// @param bits A bitsorozat, melyből kiválasztjuk a bitet
/// @param n Jobbról számítva hányadik bit
/// @return A keresett bit
Bits getBitFromRight(Bits bits, int n){
    return (Bits) {
        .b = (bits.b >> (bits.length - 1 - n)) & 1,
        .length = 1
    };
}

/// @brief Egy bitsorozatot bővít egy másik bitsorozattal jobb oldalról
/// @param bits a bővítendő bitsorozat
/// @param append A hozzáfűzendő bitsorozat
void bits_pushBit(Bits *bits, Bits append){
    //TODO mask append.b
    bits->b = bits->b << append.length | (append.b);
    bits->length += append.length;
}


/// @brief Kiír egy bitsorozatot ASCII 0 és 1 karakterekkel
/// @param bits A kiírandó bitsorozat
void print_bits(Bits bits){
    for(int i = bits.length; i > 0; i--){
        printf("%c", (bits.b >> (i-1)) & 1 ? '1' : '0');
    }
}



#endif
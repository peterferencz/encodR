#include "bin.h"

// ================================= Functions =================================

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
/// @param bits A bővítendő bitsorozat
/// @param append A hozzáfűzendő bitsorozat
void bits_pushBits(Bits *bits, Bits append){
    //TODO mask append.b
    bits->b = bits->b << append.length | (append.b);
    bits->length += append.length;
}

/// @brief Egy bitsorozatot bővít egy \p b bittel
/// @param bits A bővítendő bitsorozat
/// @param b a hozzáadott bit
void bits_pushBit(Bits *bits, char b){
    //TODO mask append.b
    bits->b = bits->b << 1 | b;
    bits->length ++;
}


/// @brief Egy bitsorozatból adja vissza a legkissebb helyiértéken álló bitet,
/// majd azt eltávolítja.
/// @param bits A bitsorozat, melyből kivesszük a bitet
/// @return 
Bits bits_popBit(Bits *bits){
    Bits b = (Bits) {
        .b = bits->b & 1,
        .length = 1
    };
    bits->b >>= 1;
    bits->length--;
    return b;
}


/// @brief Egy bitsorozatból adja vissza a legkissebb helyiértéktől számolva
/// \p length bitet, majd azokat eltávolítja.
/// @param bits A bitsorozat, melyből kivesszük a biteket
/// @param length A kivett bitek száma
/// @return 
Bits bits_popBits(Bits *bits, int length){
    Bits b = (Bits) {
        .b = bits->b & ((1 << length) - 1),
        .length = length
    };
    bits->b >>= length;
    bits->length -= length;
    return b;
}

void bits_cpy(Bits src, Bits *dest){
    dest->length = src.length;
    dest->b = src.b;
}

/// @brief Kiír egy bitsorozatot ASCII 0 és 1 karakterekkel
/// @param bits A kiírandó bitsorozat
void bits_print(Bits bits){
    for(int i = bits.length; i > 0; i--){
        printf("%c", (bits.b >> (i-1)) & 1 ? '1' : '0');
    }
}

/// @brief Összehasonlít két bitsorozatot
/// @param b1 Az összehasonlítandó bitsorozat
/// @param b2 Az összehasonlítandó bitsorozat
/// @return igaz, hogyha a bitsorozatok hossza és bitjei megegyeznek, különben hamis
bool bits_equ(Bits b1, Bits b2){
    return b1.b == b2.b && b1.length == b2.length;
}

/// @brief Megmondja, hogy egy adott bitsorozat értelmes-e.
/// @param b A vizsgálandó bitsorozat
/// @return igaz, hogyha a bitsorozat hossza 0, különben hamis
bool bits_isNullbit(Bits b){
    return b.length == 0;
}

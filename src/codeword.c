#ifndef INCL_CODEWORD
#define INCL_CODEWORD

#include <stdbool.h>
#include <stdint.h>

typedef unsigned char uchar;

typedef struct Bits
{
    long long unsigned int b;
    int length;
} Bits;

typedef struct CodeWord{
    uchar codeWord;
    Bits bits;
} CodeWord;

const Bits NULLBIT = {
    .b = 0,
    .length = 0
};

bool bits_equ(Bits b1, Bits b2){
    if(b1.b == b2.b && b1.length == b2.length){
        return true;
    }
    return false;
}

bool isNullbit(Bits b){
    return b.length == 0;
}


//TODO may be passed by reference
//TODO mask append.b
void bits_pushBit(Bits *bits, Bits append){
    bits->b = bits->b << append.length | (append.b);
    bits->length += append.length;
}

void print_bits(Bits bits){
    for(int i = bits.length; i > 0; i--){
        printf("%c", (bits.b >> (i-1)) & 1 ? '1' : '0');
    }
}

#include <stdio.h>
#include "bin.c"
void print_codeWord(CodeWord cw){
    printf("%c: ", cw.codeWord);
    print_bits(cw.bits);
    printf("\n");
}

#endif
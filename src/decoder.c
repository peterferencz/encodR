#include <stdio.h>
#include "bin.c"

// typedef struct codeWord{
//     char codeword;
//     int bitsLength;
//     char bits;
// } codeWord;

#define BUFFER_SIZE 100

void readBitsFromArr(unsigned char buffer[], int* currentBit, int size, int* to, FILE *in);

//TODO check for EOF
int decode(FILE *in, FILE *out){
    
    // char headerLength = 0;

    // fread(&headerLength, 1, sizeof(char), in);

    // printf("Dict is of size %d\n", headerLength);

    unsigned char buffer[BUFFER_SIZE];
    int currentBit = 0;

    fread(buffer, 1, BUFFER_SIZE, in);

    int headerLength = 0;
    readBitsFromArr(buffer, &currentBit, sizeof(char)*8, &headerLength, in);
    printf("header length : %d\n", headerLength);

    for(int i = 0; i < headerLength; i++){
        char codeWord = '!';
        readBitsFromArr(buffer, &currentBit, sizeof(char)*8, (int*)&codeWord, in);

        // printf("codeword: %c\n", codeWord);

        char bitsLength = '!';
        readBitsFromArr(buffer, &currentBit, sizeof(char)*8, (int*)&bitsLength, in);
        // printf("bitsLength: %d\n", bitsLength);

        char bits = '!';
        readBitsFromArr(buffer, &currentBit, bitsLength, (int*)&bits, in);

        // printf("bits: %d\n", bits);

        printf("%c: %d\n", codeWord, (int)bits);
    }
    //     char codeWord = ' ';
    //     // fread(&codeWord, 1, sizeof(char), in);


    //     char bitsLength = 0;
    //     // fread(&bitsLength, 1, sizeof(char), in);

    //     printf("Char %c is represented by \n", codeWord);
    // }

    // codeWord codewords[];


    return -1;
}

/// @brief 
/// @param buffer 
/// @param currentBit 
/// @param size in bits
/// @param to 
/// @param in 
void readBitsFromArr(unsigned char buffer[], int* currentBit, int size, int* to, FILE *in){
    int prevOffset = 0;
    // *to = 0; 

    if(size > BUFFER_SIZE * 8 - *currentBit){ //Ran out of readable bits
        prevOffset = (*currentBit + size) - BUFFER_SIZE *8;
        for(int i = 0; i < prevOffset; i++){
            // set to[0...prevOffset] = buffer[*currentbit...BUFFER_SIZE]

            int shift = 8-1-(i%8);

            to[(i)/8] = ((to[(i)/8] >> shift) + getBitFromRight(buffer[(*currentBit+i)/8], sizeof(char)*8, (*currentBit+i) % 8)) << shift;

            fread(buffer, 1, BUFFER_SIZE, in);
            *currentBit = 0;

            size -= prevOffset;
        }
    }

    for(int i = 0; i < size - prevOffset; i++){
        int shift = 8-1-(i%8);
        to[(i+prevOffset)/8] = ((to[(i+prevOffset)/8] >> shift) + getBitFromRight(buffer[(*currentBit+i)/8], sizeof(char)*8, (*currentBit+i) % 8)) << shift;
    }
    *currentBit = *currentBit+(size-prevOffset);
    
}

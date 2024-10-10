#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "codeword.c"
#include "bin.c"

typedef struct codewordFrequency{
    float freq;
    codeWord codeWord;
} codewordFrequency;


void setCode(codewordFrequency codes[], int i, int j);
char* getCode(codewordFrequency code[], int codesLength, char toEncode);
char* binToString(int bin, int length);
void wBinToString(char* str, int bin, int length);
void wBinToArr(unsigned char to[], int offset, int bin, int length);
unsigned char* constructHeader(codewordFrequency codes[], char codesLength, int *headerLength);

int compare_by_freq(const void *a, const void *b){
    return ((codewordFrequency *) b)->freq - ((codewordFrequency *) a)->freq;
}
int compare_by_bitlength(const void *a, const void *b){
    return ((codewordFrequency *) a)->codeWord.bitsLength - ((codewordFrequency *) b)->codeWord.bitsLength;
}

int encode(FILE *in, FILE *out){
    
    //presume ony characters fitting into a char
    const int size = 256;
    
    //Init array
    codewordFrequency frequencies[size];
    for(int i = 0; i < size; i++){
        frequencies[i] = (codewordFrequency){
            .freq = 0,
            .codeWord = (codeWord){
                .codeWord = (char) i,
                .bits = 0,
                .bitsLength = 0
            }
        };
    }
    
    
    char c;
    int totalCharacters = 0;
    while((c = fgetc(in)) != EOF){
        frequencies[(int)c].freq += 1;
        totalCharacters++;
    }

    qsort(frequencies, size, sizeof(codewordFrequency), compare_by_freq);

    char distinctCodeWords = 0;
    while(frequencies[(int)distinctCodeWords].freq != 0){
        distinctCodeWords++;
    }
    for(int i = 0; i < distinctCodeWords; i++){
        frequencies[i].freq /= totalCharacters;
    }

    setCode(frequencies, 0, distinctCodeWords-1);

    int headerLength = 0;
    unsigned char* header = constructHeader(frequencies, distinctCodeWords, &headerLength);
    
    fwrite(header, 1, headerLength, out);

    return 0;

    rewind(in);
    while((c = fgetc(in)) != EOF){
       char* code = getCode(frequencies, distinctCodeWords, c);
       fprintf(out, "%s", code);
       free(code);
    }

    fprintf(out, "\n");
    for(int i = 0; i < distinctCodeWords; i++){
        char* str = getCode(frequencies, distinctCodeWords, frequencies[i].codeWord.codeWord);
        fprintf(out, "%c: %4s -- %d, %d\n",frequencies[i].codeWord.codeWord, str, frequencies[i].codeWord.bits, frequencies[i].codeWord.bitsLength);
        free(str);
    }

    return 0;
}

//j > i
//codes[i] inclusive, codes[j] inclusive
void setCode(codewordFrequency codes[], int i, int j){
    float _sum = 0;
    for(int a = i; a < j; a++){
        _sum += codes[a].freq;
    }

    float half = _sum / 2;
    _sum = 0;
    int halfPointIndex = -1;

    for(int a = i; a <= j; a++){
        _sum += codes[a].freq;
        if(fabs(half - _sum) > fabs(half - _sum - codes[a+1].freq)){
            continue;
        }
        halfPointIndex = a;
        break;
    }

    //Found half-point of probabilies
    //[i-a]: 0; (a-j]: 1
    for(int a = i; a <= j; a++){
        codes[a].codeWord.bits <<= 1;
        codes[a].codeWord.bitsLength++;
        if(a > halfPointIndex){
            codes[a].codeWord.bits += 1;
        }
    }

    if(i + 1 == j){
        return;
    }

    if(halfPointIndex != i){
        setCode(codes, i, halfPointIndex);
    }
    if(j -1 != halfPointIndex){
        setCode(codes, halfPointIndex + 1,j);
    }
}


//TODO codeslengt can only have char length
//Source: https://stackoverflow.com/a/44978686/15444965 
unsigned char* constructHeader(codewordFrequency codes[], char codesLength, int *headerLength){    
    //                               | length of table            | codeword       | bitslength + bits(worst-case)
    unsigned char *bits = malloc(sizeof(int) + codesLength * (sizeof(char) + sizeof(int) * 2));
    unsigned int currentBit = 0;

    // bits[currentByte] = codesLength;
    wBinToArr(bits, currentBit, codesLength, sizeof(char)*8);
    currentBit += sizeof(char)*8;

    for(int i = 0; i < codesLength; i++){
        // Write char
        wBinToArr(bits, currentBit, codes[i].codeWord.codeWord, sizeof(codes[i].codeWord.codeWord)*8);
        currentBit += sizeof(codes[i].codeWord.codeWord)*8;

        // Write bits length
        wBinToArr(bits, currentBit, codes[i].codeWord.bitsLength, sizeof(codes[i].codeWord.bitsLength)*8);
        currentBit += sizeof(codes[i].codeWord.bitsLength)*8;

        // Write bits
        wBinToArr(bits, currentBit, codes[i].codeWord.bits, codes[i].codeWord.bitsLength);
        currentBit += codes[i].codeWord.bitsLength;
    }
    
    *headerLength = currentBit / 8;
    return bits;
}

char* getCode(codewordFrequency code[], int codesLength, char toEncode){
    for(int i = 0; i < codesLength; i++){
        if(code[i].codeWord.codeWord != toEncode){ continue; }

        return binToString(code[i].codeWord.bits, code[i].codeWord.bitsLength);
    }

    return NULL;
}

//Caller's responsibility to free
char* binToString(int bin, int length){
    char* str = malloc(length+1);
    wBinToString(str, bin, length);
    str[length] = '\0';
    return str;
}



/// @brief Write 'bin' bits to 'to' from a given 'offset'
/// @param to 
/// @param offset in bits
/// @param bin 
/// @param length 
void wBinToArr(unsigned char to[], int offset, int bin, int length){
    for(int i = 0; i < length; i++){
        int shift = 8-1-(i%8)-(offset%8);
        to[(i+offset)/8] = ((to[(i+offset)/8] >> shift) + getBitFromRight(bin, length, i)) << shift;
    }
}

/// @brief 
/// @param str 
/// @param bin 
/// @param length in bytes
void wBinToString(char* str, int bin, int length){
    for(int i = 0; i < length*8; i++){
            str[i] = getBitFromRight(bin, length*8, i) ? '1' : '0';
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cla.c"
#include "fileBuffer.c"
#include "codeword.c"
#include "bin.c"

#include "./debug.h"

typedef struct codewordFrequency{
    float freq;
    CodeWord codeWord;
} codewordFrequency;


void setCodeWord(codewordFrequency codes[], int i, int j);
Bits codewordToBits(codewordFrequency code[], int codesLength, uchar find);

int compare_by_freq(const void *a, const void *b){
    return ((codewordFrequency *) b)->freq - ((codewordFrequency *) a)->freq;
}
int compare_by_bitlength(const void *a, const void *b){
    return ((codewordFrequency *) a)->codeWord.bits.length - ((codewordFrequency *) b)->codeWord.bits.length;
}

int encode(commandLineArguments args){
    const int MAX_CHARS = 256;

    //Tömb inicializálása az összes lehetséges karakterrel
    codewordFrequency *frequencies = malloc(sizeof(codewordFrequency) * MAX_CHARS);
    if(frequencies == NULL){
        PRINTDEBUG_MALLOCNULL();
        return -1;
    }
    
    for(int i = 0; i < MAX_CHARS; i++){
        frequencies[i] = (codewordFrequency){
            .freq = 0,
            .codeWord = (CodeWord){
                .codeWord = i,
                .bits = (Bits) {
                    .b = 0,
                    .length = 0
                }
            }
        };
    }
    
    
    InputFileBuffer inputBuffer = buff_createInputFileBuffer(args.infile);
    
    
    //TODO we may overflow
    unsigned int totalCharacters = 0;

    //TODO read codeword with addbits
    Bits bits = buff_readChar(inputBuffer);
    while(!isNullbit(bits)){
        frequencies[bits.b].freq += 1;
        totalCharacters++;
        bits = buff_readChar(inputBuffer);
    }

    qsort(frequencies, MAX_CHARS, sizeof(codewordFrequency), compare_by_freq);

    int distinctCodeWords = 0;
    while(frequencies[distinctCodeWords].freq != 0){
        distinctCodeWords++;
    }


    OutputFileBuffer outputBuffer = buff_createOutputFileBuffer(args.outfile);
    buff_writeChar(outputBuffer, distinctCodeWords -1); //see documentation

    //Shrink not needed elements
    frequencies = realloc(frequencies, sizeof(codewordFrequency) * distinctCodeWords);
    if(frequencies == NULL){
        PRINTDEBUG_MALLOCNULL();
        return 1;
    }

    for(int i = 0; i < distinctCodeWords; i++){
        frequencies[i].freq /= totalCharacters;
    }
    setCodeWord(frequencies, 0, distinctCodeWords-1);
    
    
    // Store pos of padding
    // fpos_t paddingPosInFile;
    // fgetpos(outputBuffer.file, &paddingPosInFile);
    buff_writeBits(outputBuffer, (Bits) {.b = 0, .length = 3});

    if(args.displayTable && false){ printf("Kódtábla (%d elem):\n", distinctCodeWords); }
    
    for(int i = 0; i < distinctCodeWords; i++){
        buff_writeChar(outputBuffer, frequencies[i].codeWord.codeWord);
        buff_writeChar(outputBuffer, (char)frequencies[i].codeWord.bits.length);
        buff_writeBits(outputBuffer, frequencies[i].codeWord.bits);


        // --kodtabla
        if(args.displayTable && false){
            printf("%c ",frequencies[i].codeWord.codeWord);
            print_bits(frequencies[i].codeWord.bits);
            printf("\n");
        }
    }
    if(args.displayTable){ printf("\n"); }

    buff_rewind(inputBuffer);
    Bits c;
    while(!isNullbit(c = buff_readChar(inputBuffer))){
        buff_writeBits(outputBuffer, codewordToBits(frequencies, distinctCodeWords, (char) c.b));
    }

    char padding = 8 - outputBuffer.bits->length;
    buff_flush(outputBuffer);

    //Write padding
    fseek(outputBuffer.file, 1, SEEK_SET);
    uchar original;
    fread(&original, sizeof(char), 1, outputBuffer.file);
    fseek(outputBuffer.file, 1, SEEK_SET);
    uchar byte = (padding << 5) | (original & 0b00011111);
    fwrite(&byte, sizeof(char), 1, outputBuffer.file);

    free(frequencies);
    buff_destroyInputFileBuffer(inputBuffer);
    buff_destroyOutputFileBuffer(outputBuffer);
    return 0;
}

//j > i
//codes[i] inclusive, codes[j] inclusive
/// codewordFrequency --> SF-
void setCodeWord(codewordFrequency codes[], int i, int j){
    if(i == j){
        bits_pushBit(&codes[i].codeWord.bits, (Bits){
            .b = 0,
            .length = 1
        });
        return;
    }

    if(i + 1 == j){
        bits_pushBit(&codes[i].codeWord.bits, (Bits){
            .b = 0,
            .length = 1
        });
        bits_pushBit(&codes[j].codeWord.bits, (Bits){
            .b = 1,
            .length = 1
        });

        return;
    }

    float _sum = 0;
    for(int a = i; a <= j; a++){
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
        bits_pushBit(&codes[a].codeWord.bits, (Bits){
            .b = (a <= halfPointIndex) ? 0 : 1,
            .length = 1
        });
        // codes[a].codeWord.bits.b <<= 1;
        // codes[a].codeWord.bits.length++;
        // if(a > halfPointIndex){
        //     codes[a].codeWord.bits.b += 1;
        // }
    }

    

    if(halfPointIndex != i){
        setCodeWord(codes, i, halfPointIndex);
    }
    if(j -1 != halfPointIndex){
        setCodeWord(codes, halfPointIndex + 1,j);
    }
}

//TODO make this with a graph
Bits codewordToBits(codewordFrequency code[], int codesLength, uchar find){
    for(int i = 0; i < codesLength; i++){
        if(code[i].codeWord.codeWord != find){ continue; }
        return code[i].codeWord.bits;
    }


    PRINTDEBUG_CUSTOM("Karakter '%c' nincs benne a kódtáblában", find);
    return NULLBIT;
}
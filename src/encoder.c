#include "encoder.h"

// ============================ Function prototypes ============================

void setCodeWord(codewordFrequency codes[], int i, int j);
Bits codewordToBits(codewordFrequency code[], int codesLength, uchar find);
int compare_by_freq(const void *a, const void *b);
int compare_by_bitlength(const void *a, const void *b);

// ================================== Program ==================================

/// @brief Kódol Shanon-Fano algoritmus alkalmazásával egy fájlt
/// @param args Parancssori bemenet, amely a dekódolás folyamatát módosítja
/// @return 0, ha a dekódolás sikeres volt. Minden más érték sikertelen
int encode(commandLineArguments args){
    const int MAX_CHARS = 256;

    //Tömb inicializálása az összes lehetséges karakterrel
    codewordFrequency *frequencies = malloc(sizeof(codewordFrequency) * MAX_CHARS);
    CHECKMALLOCNULL(frequencies);
    
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
    while(!bits_isNullbit(bits)){
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
    CHECKMALLOCNULL(frequencies);

    for(int i = 0; i < distinctCodeWords; i++){
        frequencies[i].freq /= totalCharacters;
    }
    setCodeWord(frequencies, 0, distinctCodeWords-1);
    
    // Write padding bits (placeholder)
    buff_writeBits(outputBuffer, (Bits) {.b = 0, .length = 3});

    if(args.displayTable || args.displayStatistics){
        CodeWord *array = malloc(sizeof(CodeWord) * distinctCodeWords);
        CHECKMALLOCNULL(array);
        for(int i = 0; i < distinctCodeWords; i++){
            array[i] = frequencies[i].codeWord;
        }

        if(args.displayTable){
            stats_printCodetableArray(array, distinctCodeWords);
        }
        if(args.displayStatistics){
            stats_printCodetableStatsArray(array, distinctCodeWords);
        }
        
        free(array);
    }
    
    for(int i = 0; i < distinctCodeWords; i++){
        buff_writeChar(outputBuffer, frequencies[i].codeWord.codeWord);
        buff_writeChar(outputBuffer, (char)frequencies[i].codeWord.bits.length);
        buff_writeBits(outputBuffer, frequencies[i].codeWord.bits);
    }

    buff_rewind(inputBuffer);
    Bits c;
    while(!bits_isNullbit(c = buff_readChar(inputBuffer))){
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

// ================================= Functions =================================

/// @brief Rekurzívan beállítja egy kód tömbön az adott karakter Shanon-Fano
/// algoritmus szerinti kódját
/// @param codes A kódtömb
/// @param i A tömb kezdeti indexe (inkluzív)
/// @param j A tömb vegső indexe (inkluzív)
void setCodeWord(codewordFrequency codes[], int i, int j){
    if(i == j){
        bits_pushBit(&codes[i].codeWord.bits, 0);
        return;
    }

    if(i + 1 == j){
        bits_pushBit(&codes[i].codeWord.bits, 0);
        bits_pushBit(&codes[j].codeWord.bits, 1);

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
        bits_pushBit(&codes[a].codeWord.bits, (a <= halfPointIndex) ? 0 : 1);
    }

    if(halfPointIndex != i){
        setCodeWord(codes, i, halfPointIndex);
    }
    if(j -1 != halfPointIndex){
        setCodeWord(codes, halfPointIndex + 1,j);
    }
}

/// @brief Megkeresi \p code tömbben, \p find karaktert
/// @param code A kódtömb
/// @param codesLength A kódtömb hossza
/// @param find A keresett karakter
/// @return A keresett kódolás vagy NULLBIT
Bits codewordToBits(codewordFrequency code[], int codesLength, uchar find){
    for(int i = 0; i < codesLength; i++){
        if(code[i].codeWord.codeWord != find){ continue; }
        return code[i].codeWord.bits;
    }


    PRINTDEBUG_CUSTOM("Karakter '%c' nincs benne a kódtáblában", find);
    return NULLBIT;
}

/// @brief Frekvenciájuk alapján összehasonlít 2 frekvenciával rendelkező 
/// karakterkódolást
/// @param a Az összehasonlítandó karakterkódolás
/// @param b Az összehasonlítandó karakterkódolás
/// @return 0 = egyeznek, >0 = \p b frekvenciája nagyobb, <0 \p a 
/// frekvenciája nagyobb
int compare_by_freq(const void *a, const void *b){
    return ((codewordFrequency *) b)->freq - ((codewordFrequency *) a)->freq;
}

/// @brief Kódolásuk hossza alapján összehasonlít 2 rendelkező karakterkódolást
/// @param a Az összehasonlítandó karakterkódolás
/// @param b Az összehasonlítandó karakterkódolás
/// @return 0 = egyeznek, >0 = \p a kódja hosszabb, <0 \p b kódja hosszabb
int compare_by_bitlength(const void *a, const void *b){
    return ((codewordFrequency *) a)->codeWord.bits.length - ((codewordFrequency *) b)->codeWord.bits.length;
}
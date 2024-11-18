#include "stats.h"

// ============================ Function prototypes ============================

void printCodetableTreeRec(Node *root, Bits *_path, int *count, CodeWord *_min, CodeWord *_max, float *_avg);
long calcFileSize(const char *filename);
void prettyPrintChar(uchar c);

// ================================= Functions =================================

/// @brief Kiszámolja és kiírja két fájl mérete közötti különbséget
/// @param src A bemeneti fájl elérési útvonala
/// @param dest A kimeneti fájl elérési útvonala
void stats_printCompression(const char *src, const char *dest){
    long srcSize = calcFileSize(src);
    long destSzie = calcFileSize(dest);

    printf("Eredeti fájl mérete: %ld bájt\n", srcSize);
    printf("Kimeneti fájl mérete: %ld bájt\n", destSzie);
    printf("Tömörítés mértéke: %.3lf%%\n", ((double) srcSize / destSzie) * 100 - 100);
}

/// @brief Egy adott fájl méretét adja meg
/// @param file A fájl elérési útvonala
/// @return A fájl mérete bájtokban
long calcFileSize(const char *file){
    struct stat _stat;
    if(stat(file, &_stat) == 0){
        return _stat.st_size;
    }

    return -1;
}

/// @brief Egy fagráf esetén kiírja a kódolt karaktereket és azok kódját
/// @param root A fagráf gyökere
void stats_printCodetableTree(Node *root){
    Bits path = NULLBIT;
    CodeWord min = (CodeWord){.codeWord = root->codeword, .bits = NULLBIT};
    CodeWord max = (CodeWord){.codeWord = root->codeword, .bits = NULLBIT};
    float avg = 0;
    int cnt = 0;
    
    printf("Kódolt karakterek:\n");
    printCodetableTreeRec(root, &path, &cnt, &min, &max, &avg);
    avg /= cnt;

    printf("Statisztika:\nÖsszesen %d kód\n", cnt);
    printf("Legrövidebb: ");
    prettyPrintChar(min.codeWord);
    printf(": (");
    bits_print(min.bits);
    printf(")\nLeghosszabb: ");
    prettyPrintChar(max.codeWord);
    printf(": (");
    bits_print(max.bits);
    printf(")\nÁtlagos méret: %.3f\n", avg);

}

/// @brief Rekurzívan kiírja egy bináris fagráf esetén a kódolt karaktereket
/// és azok kódját. A \ref stats_printCodetableTree segédfügvénye
/// @param root A fagráf gyökere
/// @param _path A fagráf jelenlegi elérési útvonala
/// @param _count A fagráf beállított elemeinek száma
/// @param _min A fagráf legrövidebben kódolt karaktere
/// @param _max A fagráf leghosszabban kódolt karaktere
/// @param _avg A kódolt karakterek hosszának átlaga
void printCodetableTreeRec(Node *root, Bits *_path, int *_count, CodeWord *_min, CodeWord *_max, float *_avg){
    if(root == NULL){ return; }
    
    if(root->set){
        prettyPrintChar(root->codeword);
        printf(": ");
        bits_print(*_path);
        printf("\n");

        if(_path->length < _min->bits.length || bits_isNullbit(_min->bits)){
            _min->codeWord = root->codeword;
            _min->bits = *_path;
        } else if(_max->bits.length < _path->length || bits_isNullbit(_max->bits)){
            _max->codeWord = root->codeword;
            _max->bits = *_path;
        }

        ++*_count;
        *_avg += _path->length;
    }
    
    bits_pushBit(_path, 0);
    printCodetableTreeRec(root->left_0, _path, _count, _min, _max, _avg);
    bits_popBit(_path);

    bits_pushBit(_path, 1);
    printCodetableTreeRec(root->right_1, _path, _count, _min, _max, _avg);
    bits_popBit(_path);
}

/// @brief Egy kódtömb esetén kiírja a kódolt karaktereket és azok kódját
/// @param array A kódtömb
/// @param elements A tömb elemeinek száma
void stats_printCodetableArray(CodeWord array[], int elements){
    printf("Kódolt karakterek:\n");

    for(int i = 0; i < elements; i++){
        prettyPrintChar(array[i].codeWord);
        printf(": ");
        bits_print(array[i].bits);
        printf("\n");
    }

    
}

/// @brief Kiírja egy kódtömb statisztikáját
/// @param array A kódtömb
/// @param elements A tömb elemeinek száma
void stats_printCodetableStatsArray(CodeWord array[], int elements){
    int minI = 0;
    int maxI = 0;
    float avg = 0;

    for(int i = 0; i < elements; i++){
        if(array[i].bits.length < array[minI].bits.length){
            minI = i;
        } else if(array[maxI].bits.length < array[i].bits.length){
            maxI = i;
        }
        avg += (float)array[i].bits.length / elements;
    }

    printf("Statisztika:\nÖsszesen %d kód\n", elements);
    printf("Legrövidebb: ");
    prettyPrintChar(array[minI].codeWord);
    printf(": (");
    bits_print(array[minI].bits);
    printf(")\nLeghosszabb: ");
    prettyPrintChar(array[maxI].codeWord);
    printf(": (");
    bits_print(array[maxI].bits);
    printf(")\nÁtlagos méret: %.3f\n", avg);
}

/// @brief Egy karakter beszédesebb formáját írja ki a standard kimenetre
/// @param c A kiírandó karakter
void prettyPrintChar(uchar c){
    // Data sourced from Wikipedia: ASCII (https://en.wikipedia.org/wiki/ASCII)
    const char* chars[] = {"NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL","BS","HT","LF","VT","FF","CR","SO","SI","1x","DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB","CAN","EM","SUB","ESC","FS","GS","RS","US","2x","SP"};
    if(c < 35){
        printf("%s", chars[c]);
        return;
    }
    switch (c) {
        case '\n': printf("\\n"); break;
        case '\t': printf("\\t"); break;
        default: printf("%c", c); break;
    }
}
#include "decoder.h"

// ============================ Function prototypes ============================

void appendCodeword(Node *root, Bits codeword, char set);
Node *createNodeIfNotexists(Node *parent, int dir);
void displayTable(Node *root, Bits *_path);
void freeTree(Node *root);

// ================================== Program ==================================

/// @brief Dekódol egy Shanon-Fano algoritmussal kódolt fájlt
/// @param args Parancssori bemenet, amely a dekódolás folyamatát módosítja
/// @return 0, ha a dekódolás sikeres volt. Minden más érték sikertelen
int decode(commandLineArguments args){
    InputFileBuffer inputBuffer = buff_createInputFileBuffer(args.infile);

    Bits btableLength = buff_readChar(inputBuffer);
    if(bits_isNullbit(btableLength)){
        PRINTDEBUG_CORRUPTEDFILE()
        return -1;
    }
    // +1 see documentation
    int tableLength = (int) btableLength.b + 1;

    Bits bpaddingLength = buff_readBits(inputBuffer, 3);
    if(bits_isNullbit(bpaddingLength)){
        PRINTDEBUG_CORRUPTEDFILE()
        return -1;
    }
    int padding = (int) bpaddingLength.b;
    if(padding < 0 || padding >= 8){
        PRINTDEBUG_CORRUPTEDFILE()
        return -1;
    }

    Node *root = malloc(sizeof(Node));
    PRINTDEBUG_CUSTOM("MALLOC: %p", root);
    CHECKMALLOCNULL(root);

    root->left_0 = NULL;
    root->right_1 = NULL;
    root->set = false;
    root->codeword = '\0';


    for(int i = 0; i < tableLength; i++){
        Bits cb = buff_readChar(inputBuffer);
        if(bits_isNullbit(cb)){
            PRINTDEBUG_CORRUPTEDFILE()
            return -1;
        }
        char c = (char)cb.b;

        Bits lb = buff_readChar(inputBuffer);
        if(bits_isNullbit(lb)){
            PRINTDEBUG_CORRUPTEDFILE()
            return -1;
        }
        char l = (char)lb.b;

        Bits codeb = buff_readBits(inputBuffer, l);
        if(bits_isNullbit(codeb)){
            PRINTDEBUG_CORRUPTEDFILE()
            return -1;
        }

        appendCodeword(root, codeb, c);
    }


    if(args.displayTable){
        stats_printCodetableTree(root);
    }

    OutputFileBuffer outputBuffer = buff_createOutputFileBuffer(args.outfile);

    char rotBuf[8] = {0};
    int rotBufi = 0;
    bool rotBufFilled = false;

    Bits bit;
    Node *current = root;
    while(!bits_isNullbit(bit = buff_readBit(inputBuffer))){
        char dir = bit.b;

        if(dir){
            if(current->right_1 == NULL){
                if(rotBufFilled){
                    buff_writeChar(outputBuffer, rotBuf[rotBufi]);
                }
                if(rotBufi == 7) { rotBufFilled = true; }
                rotBuf[rotBufi] = current->codeword;
                rotBufi = (rotBufi + 1) % 8;
                current = root;
            }
            current = current->right_1;
        }else{
            if(current->left_0 == NULL){
                if(rotBufFilled){
                    buff_writeChar(outputBuffer, rotBuf[rotBufi]);
                }
                if(rotBufi == 7) { rotBufFilled = true; }
                rotBuf[rotBufi] = current->codeword;
                rotBufi = (rotBufi + 1) % 8;
                current = root;
            }
            current = current->left_0;
        }
    }
    //Last char
    if(rotBufFilled){
        buff_writeChar(outputBuffer, rotBuf[rotBufi]);
    }
    if(rotBufi == 7) { rotBufFilled = true; }
    rotBuf[rotBufi] = current->codeword;
    rotBufi = (rotBufi + 1) % 8;

    int zeroDepth = 0;
    current = root;
    while(current != NULL){
        current = current->left_0;
        zeroDepth++;
    }
    zeroDepth--;
    
    int extraCharCount = padding / zeroDepth + (padding % zeroDepth != 0);
    int n = (rotBufFilled ? 8 : rotBufi) - extraCharCount;
    for(int i = 0; i < n; i++){
        buff_writeChar(outputBuffer, rotBuf[rotBufFilled ? rotBufi : i]);
        rotBufi = (rotBufi + 1) % 8;
    }

    freeTree(root);
    buff_destroyInputFileBuffer(inputBuffer);
    buff_destroyOutputFileBuffer(outputBuffer);
    return 0;
}

// ================================= Functions =================================

/// @brief Egy fagráfhoz, \p codeword bitjeinek bejárása alapján beállítja egy
/// elem kódolt karakterét. Ha az adott elérés nem létezik, a függvény
/// létrehozza azt.
/// @param root A fagráf gyökere
/// @param codeword A bejárás bitjei: 0 = bal, 1 (minden más) = jobb
/// @param set A beállítandó karakter, melyhez elérkeztünk a bejárás végén
void appendCodeword(Node *root, Bits codeword, char set){
    Node *current = root;

    for(int i = 0; i <  codeword.length; i++){
        char c = getBitFromRight(codeword, i).b;

        current = createNodeIfNotexists(current, c);
    }
    current->codeword = set;
    current->set = true;
}

/// @brief Egy fagráf adott eleméből megpróbál \p dir által meghatároott úton
/// továbbhaladni. Ha az nem létezik, létrehozza azt.
/// @param parent Az elem, melyből kiindulunk
/// @param dir Az irány: 0 = bal, 1 (minden más) = jobb
/// @return A gráf azon eleme, mely \p parent -től \p dir irányba helyezkedik el
Node *createNodeIfNotexists(Node *parent, int dir){
    if(dir == 0){
        if(parent->left_0 == NULL){
            Node *new = malloc(sizeof(Node));
            CHECKMALLOCNULL(new);
            new->left_0 = NULL;
            new->right_1 = NULL;
            new->codeword = '\0';
            new->set = false;
            parent->left_0 = new;
        }
        return parent->left_0;
    }else{
        if(parent->right_1 == NULL){
            Node *new = malloc(sizeof(Node));
            CHECKMALLOCNULL(new);
            new->left_0 = NULL;
            new->right_1 = NULL;
            new->set = false;
            parent->right_1 = new;
        }
        return parent->right_1;
    }
}


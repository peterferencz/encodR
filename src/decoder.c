#include <stdio.h>
#include <stdbool.h>

#include "debug.h"

#include "cla.c"
#include "bin.c"
#include "fileBuffer.c"
#include "codeword.c"
#include "graph.c"

bool appendCodeword(Node *root, Bits codeword, char set);
Node *createNodeIfNotexists(Node *parent, int dir);
Node *createNodeIfNotexists(Node *parent, int dir);
void freeTree(Node *root);

int decode(commandLineArguments args){
    InputFileBuffer inputBuffer = buff_createInputFileBuffer(args.infile);

    Bits btableLength = buff_readChar(inputBuffer);
    if(isNullbit(btableLength)){
        PRINTDEBUG_CORRUPTEDFILE()
        return -1;
    }
    // +1 see documentation
    int tableLength = (int) btableLength.b + 1;

    Bits bpaddingLength = buff_readBits(inputBuffer, 3);
    if(isNullbit(bpaddingLength)){
        PRINTDEBUG_CORRUPTEDFILE()
        return -1;
    }
    int padding = (int) bpaddingLength.b;
    if(padding < 0 || padding >= 8){
        PRINTDEBUG_CORRUPTEDFILE()
        return -1;
    }

    Node *root = malloc(sizeof(Node));
    if(root == NULL){
        PRINTDEBUG_MALLOCNULL();
        return -1;
    }
    root->left_0 = NULL;
    root->right_1 = NULL;
    root->codeword = '\0';


    for(int i = 0; i < tableLength; i++){
        Bits cb = buff_readChar(inputBuffer);
        if(isNullbit(cb)){
            PRINTDEBUG_CORRUPTEDFILE()
            return -1;
        }
        char c = (char)cb.b;

        Bits lb = buff_readChar(inputBuffer);
        if(isNullbit(lb)){
            PRINTDEBUG_CORRUPTEDFILE()
            return -1;
        }
        char l = (char)lb.b;

        Bits codeb = buff_readBits(inputBuffer, l);
        if(isNullbit(codeb)){
            PRINTDEBUG_CORRUPTEDFILE()
            return -1;
        }

        appendCodeword(root, codeb, c);
    }


    if(args.displayTable){
        //TODO display table
    }

    OutputFileBuffer outputBuffer = buff_createOutputFileBuffer(args.outfile);

    char rotBuf[8] = {0};
    int rotBufi = 0;
    bool rotBufFilled = false;

    Bits bit;
    Node *current = root;
    while(!isNullbit(bit = buff_readBit(inputBuffer))){
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
    
    int extraCharCount = padding / zeroDepth + (padding % zeroDepth != 0); // +1 due to writing out last
    // rotBufi = (rotBufi + 7) % 8;
    int n = (rotBufFilled ? 8 : rotBufi) - extraCharCount;
    for(int i = 0; i < n; i++){
        buff_writeChar(outputBuffer, rotBuf[rotBufFilled ? rotBufi : i]);
        // rotBuf[rotBufi] = current->codeword;
        rotBufi = (rotBufi + 1) % 8;
    }
    

    freeTree(root);
    buff_destroyInputFileBuffer(inputBuffer);
    buff_destroyOutputFileBuffer(outputBuffer);
    return 0;
}

bool appendCodeword(Node *root, Bits codeword, char set){
    Node *current = root;

    for(int i = 0; i <  codeword.length; i++){
        char c = getBitFromRight(codeword, i);

        current = createNodeIfNotexists(current, c);
    }
    current->codeword = set;

    return true;
}

void freeTree(Node *root){
    if(root->left_0 != NULL){
        freeTree(root->left_0);
    }
    if(root->right_1 != NULL){
        freeTree(root->right_1);
    }
    free(root);
}

// 0 : left, 1 (default) : right
Node *createNodeIfNotexists(Node *parent, int dir){
    if(dir == 0){
        if(parent->left_0 == NULL){
            Node *new = malloc(sizeof(Node));
            if(new == NULL){
                PRINTDEBUG_MALLOCNULL();
                return NULL;
            }
            new->left_0 = NULL;
            new->right_1 = NULL;
            new->codeword = '\0';
            parent->left_0 = new;
        }
        return parent->left_0;
    }else{
        if(parent->right_1 == NULL){
            Node *new = malloc(sizeof(Node));
            if(new == NULL){
                PRINTDEBUG_MALLOCNULL();
                return NULL;
            }
            new->left_0 = NULL;
            new->right_1 = NULL;
            parent->right_1 = new;
        }
        return parent->right_1;
    }
}

// Node *constructTableFromFile(InputFileBuffer input) {
//     InputFileBuffer tableFile = buff_createInputFileBuffer(file);
    
    

//     int sor = 1;


//     while(true){
//         char cw;
//         Bits read = buff_readChar(tableFile);
//         if(isNullbit(read)){break;}
//         cw = (char) read.b;
//         if(cw == '\n') { //Double newline
//             return root;
//         }

//         //Szóköz
//         read = buff_readChar(tableFile);
//         if(isNullbit(read)){
//             PRINTDEBUG_CUSTOM("Nem jó formátumú kódtábla (befejezetlen a %d. sor)", sor);
//             return NULL;
//         }
//         char separator = (char) read.b;
//         if(separator != ' '){
//             PRINTDEBUG_CUSTOM("Nem szóköz választja el a karaktert a kódjától (%d.sor)", sor);
//             return NULL;
//         }
        
//         Node *current = root;
//         bool nlfound = false;

//         while(!nlfound){
//             read = buff_readChar(tableFile);
//             if(isNullbit(read)){
//                 PRINTDEBUG_CUSTOM("Nem jó formátumú kódtábla (befejezetlen a %d. sor)", sor);
//                 return NULL;
//             }
//             char c = (char)read.b;
            
//             switch(c){
//                 case '0':
//                     current = createNodeIfNotexists(current, 0);
//                     break;
//                 case '1':
//                     current = createNodeIfNotexists(current, 1);
//                     break;
//                 case '\n':
//                     if(current == root){
//                         PRINTDEBUG_CUSTOM("Nem jó formátumú kódtábla (befejezetlen a %d. sor)", sor);
//                         return NULL;
//                     }

//                     current->codeword = cw;
//                     sor++;
//                     nlfound = true;
//                     break;
//                 default:
//                     PRINTDEBUG_CUSTOM("Nem megengedett karakter '%c' a kódtáblában (%d.sor)", (char)read.b, sor);
//                     return NULL;
//             }
//         }
//     }
//     return root;
// }



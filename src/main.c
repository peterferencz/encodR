#include <stdio.h>
#include <string.h>
#include "./encoder.c"
#include "./decoder.c"

enum mode {
    ENCODE = 0,
    DECODE = 1
};
typedef struct parsedArguments {
    enum mode m;
    char* input;
    char* output;
} parsedArguments;

int parseArgs(int argc, char** argv, parsedArguments *args);

int main(int argc, char** argv) {

    parsedArguments args;
    if(parseArgs(argc, argv, &args) != 0){
        printf("Hasznalat: shanon <kodol|dekodol> <bemeneti fajl> <kimeneti fajl>\n");
        return 0;
    }
    
    FILE *infile = fopen(args.input, args.m == ENCODE ? "r" : "rb");
    if(infile == NULL){
        printf("Nem sikerult megnyitni a fajlt:  %s\n", args.input);
        return 1;
    }
    FILE *outfile = fopen(args.output, args.m == ENCODE ? "wb" : "w");
    if(infile == NULL){
        printf("Nem sikerult megnyitni a fajlt: %s\n", args.output);
        return 1;
    }

    if(args.m == ENCODE){
        encode(infile, outfile);
    }else{
        decode(infile, outfile);
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}

// huffman tömörítés 
// dekódoló

// karakterkészlet - számok / ascii

// kódolás mértéke


// kódolt szöveg --> mivel lett kódolva (vissza lehet-e állítani a kódtáblát)

int parseArgs(int argc, char** argv, parsedArguments *args){
    if(argc <= 3) {
        return -1;
    }

    if(strcmp(argv[1], "kodol") == 0) {
        args->m = ENCODE;
    } else if(strcmp(argv[1], "dekodol") == 0) {
        args->m = DECODE;
    }else {
        return -1;
    }

    args->input = argv[2];
    args->output = argv[3];
    
    return 0;
}

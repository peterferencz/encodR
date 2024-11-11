// ================================== Headers ==================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "./debug/debug.h"

#include "cla.c"
#include "./encoder.c"
#include "./decoder.c"

// ================================== Structs ==================================

/// @brief Megadja, hogy a program a 'kodol' vagy 'dekodol' paraméterrel lett
///        meghívva
enum MODE {
    ENCODE = 0,
    DECODE = 1,
    UNSET = -1
};

// ============================ Function prototypes ============================

void printHelp();
int parseCLA(int argc, char** argv, commandLineArguments *args, enum MODE *mode);

// ================================== Program ==================================

//TODO stderr
//TODO check each buff_write for error
int main(int argc, char** argv) {
    commandLineArguments args;
    enum MODE mode = UNSET;

    if(parseCLA(argc, argv, &args, &mode) != 0){
        return -1;
    }

    int returnCode = 0;
    if(mode == ENCODE){
        returnCode = encode(args);
    }else{
        returnCode = decode(args);
    }

    if(args.infile != stdin){
        fclose(args.infile);
    }
    if(args.outfile != stdout){
        fclose(args.outfile);
    }

    return returnCode;
}

// ================================= Functions =================================

void printHelp(){
    printf(
        "program [üzemmód] <...kapcsolók...>\n"
        "Üzemmód: kodol, dekodol\n"
        "Kapcsolók:\n"
        "--bemenet <forrásfájl>: Bemeneti fájl (ha üres akkor stdin)\n"
        "--kimenet <célfájl>: Bemeneti fájl (ha üres akkor stdout)\n"
        "--kodtabla <fájl>: A kódtábla fájl (kötelező)\n"
        "--statisztika: A tömörítés hatékonyságát értékelő statisztika (opcionális)\n"
        "--help: Ezt az üzenetet írja ki (opcionális)\n"
    );
}

/// @brief A bemeneti paraméterek feldolgozására szolgáló függvény
/// @param argc 'argv' hossza 
/// @param argv parancssori argumentumok
/// @param args 
/// @return 0, ha sikeres volt az argumentumok elemzése, különben ettől eltérő
//          érték
int parseCLA(int argc, char** argv, commandLineArguments *args, enum MODE *mode){
    args->displayStatistics = false;
    args->displayTable = false;
    *mode = UNSET;
    
    static struct option long_options[] = {
        {"bemenet", required_argument, NULL, 'i'},
        {"kimenet", required_argument, NULL, 'o'},
        {"kodtabla", no_argument, NULL, 't'},
        {"statisztika", no_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, '\0'}
    };

    char* infile = "";
    char* outfile = "";


    //Processing flags
    char ch;
    while ((ch = getopt_long_only(argc, argv, "i:o:ts", long_options, NULL)) != -1) {
    switch (ch) {
        case 'i': infile = optarg; break;
        case 'o': outfile = optarg; break;
        case 't': args->displayTable = true; break;
        case 's': args->displayStatistics = true; break;
        case 'h': printHelp(); return -1;
        default: break;
        }
    }

    //Unprocessed args
    while(optind < argc){
        if(strcmp(argv[optind], "kodol") == 0) {
            *mode = ENCODE;
        } else if(strcmp(argv[optind], "dekodol") == 0) {
            *mode = DECODE;
        }else {
            printf("Ismeretlen paraméter: %s\n", argv[optind]);
            return -1;
        }
        optind++;
    }

    if(*mode == UNSET){
        printf("Mód nincs beállítva\n");
        return -1;
    }

    FILE *in;
    if(*infile == '\0'){
        printf("Bemenet kapcsoló nincs beállítva, alapértelmezett érték: 'stdin'\n");
        in = stdin;
    }else{
        in = fopen(infile, "rb+");
        if(in == NULL){
            printf("Nem sikerult megnyitni a fajlt:  %s\n", infile);
            return -1;
        }
    }
    
    FILE *out;
    if(*outfile == '\0'){
        printf("Kimenet kapcsoló nincs beállítva, alapértelmezett érték: 'stdout'\n");
        out = stdin;
    }else{
        out = fopen(outfile, "wb+");
        if(out == NULL){
            printf("Nem sikerult megnyitni a fajlt:  %s\n", outfile);
            return -1;
        }
    }

    args->infile = in;
    args->outfile = out;

    return 0;
}

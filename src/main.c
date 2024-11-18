#include "main.h"

// ============================ Function prototypes ============================

void printHelp();
int parseCLA(int argc, char** argv, commandLineArguments *args);

// ================================== Program ==================================

/// @brief A program belépési pontja
/// @param argc \p argv parancssori argumentumok hossza
/// @param argv parancssori argumentumok
/// @return A program futásának eredménye.
/// 0 = Rendeltetésszerű futás, bármi más esetén sikertelen a program futása
int main(int argc, char** argv) {
    commandLineArguments args;

    if(parseCLA(argc, argv, &args) != 0){
        return -1;
    }

    int returnCode = 0;
    if(args.mode == ENCODE){
        returnCode = encode(args);
    }else{
        returnCode = decode(args);
    }

    if(args.displayStatistics && returnCode == 0){
        stats_printCompression(args.infile, args.outfile);
    }

    return returnCode;
}

// ================================= Functions =================================

/// @brief Kiírja a standard outputra a program elfogadott paramétereket és
/// kapcsolókat
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

/// @brief A parancssori argumentumokat állítja be \p args paramétereként és
/// alakítja át azokat \ref commandLineArguments kapcsolókat feldolgozó függvény
/// @param argc \p argc hossza
/// @param argv A programnak átadott parancssori paraméterek
/// @param args A függvény ide tölti be a feldolgozott kapcsolókat.
/// \p args értéke megváltozhat annak ellenére, hogy a függvény nem 0 kimenettel
/// tér vissza
/// @return 0, ha sikeres volt az argumentumok elemzése, és minden kötelező 
/// paraméter meg lett adva különben ettől eltérő érték
int parseCLA(int argc, char** argv, commandLineArguments *args){
    args->displayStatistics = false;
    args->displayTable = false;
    args->mode = UNSET;
    
    //Define available options
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

    //Handle unprocessed parameters
    while(optind < argc){
        if(strcmp(argv[optind], "kodol") == 0) {
            args->mode = ENCODE;
        } else if(strcmp(argv[optind], "dekodol") == 0) {
            args->mode = DECODE;
        }else {
            printf("Ismeretlen paraméter: %s\n", argv[optind]);
            return -1;
        }
        optind++;
    }

    if(args->mode == UNSET){
        printf("Mód nincs beállítva\n");
        return -1;
    }

    if(*infile == '\0'){
        printf("Bemenet kapcsoló nincs beállítva, alapértelmezett érték: 'stdin'\n");
    }
    
    if(*outfile == '\0'){
        printf("Kimenet kapcsoló nincs beállítva, alapértelmezett érték: 'stdout'\n");
    }

    args->infile = infile;
    args->outfile = outfile;

    return 0;
}

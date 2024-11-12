#include "codeword.h"

// /// @brief Előjel nélküli 8 bites karakter
// typedef unsigned char uchar;

// /// @brief Tetszőleges hosszú bitsorozat eltárolására alkalmas struktúra
// typedef struct Bits {
//     /// @brief A tárolt szám
//     /// A bitek jobbról balra értelmezendőek
//     long long unsigned int b;

//     /// @brief A tárolt bitsorozat hossza
//     size_t length;
// } Bits;

// /// @brief Karakter, és az azt kódoló bitsorozat
// typedef struct CodeWord{

//     /// @brief Egy byte, melyet a Shanon-Fano kódolás szerint kódolunk
//     uchar codeWord;

//     /// @brief Egy bitsorozat, melyet a Shanon-Fano kódolás szerint a @ref{ codeWord } kódolt változata
//     Bits bits;
// } CodeWord;

// /// @brief Hibás kimenetet jelentő bitsorozat, melynek hossza 0
// const Bits NULLBIT = {
//     .b = 0,
//     .length = 0
// };

/// @brief Összehasonlít két bitsorozatot
/// @param b1 Az összehasonlítandó bitsorozat
/// @param b2 Az összehasonlítandó bitsorozat
/// @return igaz, hogyha a bitsorozatok hossza és bitjei megegyeznek, különben hamis
bool bits_equ(Bits b1, Bits b2){
    return b1.b == b2.b && b1.length == b2.length;
}

/// @brief Megmondja, hogy egy adott bitsorozat értelmes-e.
/// @param b A vizsgálandó bitsorozat
/// @return igaz, hogyha a bitsorozat hossza 0, különben hamis
bool isNullbit(Bits b){
    return b.length == 0;
}

#include "fileBuffer.h"

// ================================= Functions =================================

/// @brief Készít egy bitek olvasására alkalmas puffert
/// @param path A fájl elérési útvonala
/// @return Az elkészített puffer
InputFileBuffer buff_createInputFileBuffer(const char *path){
    uchar *cb = malloc(sizeof(uchar));
    CHECKMALLOCNULL(cb);

    FILE *file = fopen(path, "rb+");
    if(file == NULL){
        PRINTDEBUG_FILEERR();
        exit(1); //TODO return with error rather than exit
    }

    clearerr(file);
    *cb = 0;

    return (InputFileBuffer) {
        .currentBit = cb,
        .file = file
    };
}

/// @brief Készít egy bitek írására alkalmas puffert
/// @param path A fájl elérési útvonala
/// @return Az elkészített puffer
OutputFileBuffer buff_createOutputFileBuffer(const char *path){
    Bits *bits = malloc(sizeof(Bits));
    CHECKMALLOCNULL(bits);

    FILE *file = fopen(path, "wb+");
    if(file == NULL){
        PRINTDEBUG_FILEERR();
        exit(1); //TODO return with error rather than exit
    }

    clearerr(file);
    *bits = NULLBIT;

    return (OutputFileBuffer) {
        .file = file,
        .bits = bits
    };
}

/// @brief Bezárja a puffer által megynyitott fájlt, és felszabadítja az az
/// által lefoglalt memóriát
/// @param buffer A felszabadítandó puffer
void buff_destroyInputFileBuffer(InputFileBuffer buffer){
    free(buffer.currentBit);
    buffer.currentBit = NULL;
    fclose(buffer.file);
}

/// @brief Bezárja a puffer által megynyitott fájlt, és felszabadítja az az
/// által lefoglalt memóriát
/// @param buffer A felszabadítandó puffer
void buff_destroyOutputFileBuffer(OutputFileBuffer buffer){
    free(buffer.bits);
    buffer.bits = NULL;
    fclose(buffer.file);
}

/// @brief A fájl újboli olvasására készíti fel a puffer
/// @param buffer A visszahízandó puffer
void buff_rewind(InputFileBuffer buffer){
    rewind(buffer.file);
    *buffer.currentBit = 0;
}

// ================================== Writing ==================================

/// @brief Egy fájlba ír biteket
/// @param buff A puffer, melybe írunk
/// @param bit Egy tetszőleges hosszúságú bitsorozat
/// @return 'false', ha sikeres a művelet, különben 'true'
bool buff_writeBits(OutputFileBuffer buff, Bits bit){
    
    bits_pushBits(buff.bits, bit);


    if(buff.bits->length >= 8){
        int r = buff.bits->length % 8;
        unsigned long long d = buff.bits->b >> r;
        fwrite(&d, sizeof(uchar), buff.bits->length / 8, buff.file);

        if(ferror(buff.file) != 0){
            PRINTDEBUG_FILEERR();
            return true;
        }

        int mask = (1 << r) - 1;
        *buff.bits = (Bits){
            .b = buff.bits->b & mask,
            .length = r
        };
    }
    return false;
}

/// @brief Egy fájlba ír 1 bitet
/// @param buff A puffer, melybe írunk
/// @param bit A beírandó bit. Hogyha a bitsorozat hossza nem 1, akkor a
// függvény úgy funkcionál, mint \ref{buff_writeBits}
/// @return 'false', ha sikeres a művelet, különben 'true'
bool buff_writeBit(OutputFileBuffer buff, Bits bit){
    #ifdef DEBUG
    if(bit.length != 1){
        PRINTDEBUG_CUSTOM("%s was called with 'bit' whos length is not 1", __FUNCTION__);
    }
    #endif
    return buff_writeBits(buff, bit);
}

/// @brief Egy fájlba ír 1 karaktert
/// @param buff A puffer, melybe írunk
/// @param val A beírandó érték
/// @return 'false', ha sikeres a művelet, különben 'true'
bool buff_writeChar(OutputFileBuffer buff, uchar val){
    return buff_writeBits(buff, (Bits){.b = val, .length = 8 * sizeof(uchar)});
}

/// @brief Egy fájlba ír 1 egész számot
/// @param buff A puffer, melybe írunk
/// @param val A beírandó érték
/// @return 'false', ha sikeres a művelet, különben 'true'
bool buff_writeInt(OutputFileBuffer buff, int val){
    return buff_writeBits(buff, (Bits){.b = val, .length = 8 * sizeof(int)});
}

/// @brief Beírja a fájlba a puffer tartalmát, 0val kiegészítve
/// @param buff A puffer, melybe írunk
/// @return 'false', ha sikeres a művelet, különben 'true'
bool buff_flush(OutputFileBuffer buff){
    if(buff.bits->length == 0) { return true; }
    int remaining = 8 - buff.bits->length;
    
    return buff_writeBits(buff, (Bits){
        .b = 0,
        .length = remaining
    });
}

// ================================== Reading ==================================

/// @brief Egy fálból olvas 1 bitet
/// @param buff A puffer, amiből olvasunk
/// @return #NULLBIT, ha EOF vagy fájl olvasási hiba lépett fell, különben az
/// olvasott bit
Bits buff_readBit(InputFileBuffer buff){
    uchar data = 0;
    if(fread(&data, 1, 1, buff.file) == 0){
        if(feof(buff.file) != 0){
            return NULLBIT;
        }
        if(ferror(buff.file) != 0){
            PRINTDEBUG_FILEERR();
            return NULLBIT;
        }
    }
    
    uchar b = data >> (7 - *buff.currentBit) & 1;
    ++*buff.currentBit;

    if(*buff.currentBit / 8 < 1){
        //TODO Use stored char rather than seeking
        fseek(buff.file, -1, SEEK_CUR);
    }else{
        *buff.currentBit %= 8;
    }

    return (Bits) {
        .b = b,
        .length = 1
    };
}

/// @brief Egy fálból olvas \p bitCount darab bitet
/// @param buff A puffer, amiből olvasunk
/// @param bitCount Hány darab bitet olvassunk
/// @return A beolvasott bitsorozat
Bits buff_readBits(InputFileBuffer buff, int bitCount){
    Bits b = { .b = 0, .length = 0 };

    int bitsRead = 0;

    while(bitsRead < bitCount){
        Bits bit = buff_readBit(buff);
        if(bits_isNullbit(bit)){ return NULLBIT; }
        
        bits_pushBits(&b, bit);

        ++bitsRead;
    }

    return b;
}

/// @brief Egy fálból olvas 1 karaktert
/// @param buff A puffer, amiből olvasunk
/// @return A beolvasott karakter
Bits buff_readChar(InputFileBuffer buff){
    return buff_readBits(buff, 8);
}

/// @brief Egy fálból olvas 1 egész számot
/// @param buff A puffer, amiből olvasunk
/// @return A beolvasott szám
Bits buff_readInt(InputFileBuffer buff){
    return buff_readBits(buff, sizeof(int)*8);
}

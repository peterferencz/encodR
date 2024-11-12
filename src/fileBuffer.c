#include "fileBuffer.h"

// ================================= Functions =================================

/// @brief 
/// @param file Az olvasandó file stream-je
/// @return A kreált puffer
InputFileBuffer buff_createInputFileBuffer(FILE *file){
    uchar *cb = malloc(sizeof(uchar));
    if(cb == NULL){
        PRINTDEBUG_MALLOCNULL();
        exit(1); //TODO return with error rather than exit
    }

    clearerr(file);
    *cb = 0;

    return (InputFileBuffer) {
        .currentBit = cb,
        .file = file
    };
}

OutputFileBuffer buff_createOutputFileBuffer(FILE *file){
    Bits *bits = malloc(sizeof(Bits));
    if(bits == NULL){
        PRINTDEBUG_MALLOCNULL();
        exit(1); //TODO return with error rather than exit
    }

    clearerr(file);
    *bits = NULLBIT;

    return (OutputFileBuffer) {
        .file = file,
        .bits = bits
    };
}

//Does not close file
void buff_destroyInputFileBuffer(InputFileBuffer buffer){
    free(buffer.currentBit);
    buffer.currentBit = NULL;
}

//Does not close file
void buff_destroyOutputFileBuffer(OutputFileBuffer buffer){
    free(buffer.bits);
    buffer.bits = NULL;
}


void buff_rewind(InputFileBuffer buffer){
    rewind(buffer.file);
    *buffer.currentBit = 0;
}

//0: ok, other(1): ferror
bool buff_writeBits(OutputFileBuffer buff, Bits bit){
    
    bits_pushBit(buff.bits, bit);


    if(buff.bits->length >= 8){
        int r = buff.bits->length % 8;
        unsigned long long d = buff.bits->b >> r;
        fwrite(&d, sizeof(uchar), buff.bits->length / 8, buff.file);

        if(ferror(buff.file) != 0){
            PRINTDEBUG_FILEERR();
            return 1;
        }

        int mask = (1 << r) - 1;
        *buff.bits = (Bits){
            .b = buff.bits->b & mask,
            .length = r
        };
    }
    return 0;
}

bool buff_writeBit(OutputFileBuffer buff, Bits bit){
    #ifdef DEBUG
    if(bit.length != 1){
        PRINTDEBUG_CUSTOM("%s was called with 'bit' whos length is not 1", __FUNCTION__);
    }
    #endif
    return buff_writeBits(buff, bit);
}

bool buff_writeChar(OutputFileBuffer buff, uchar val){
    return buff_writeBits(buff, (Bits){.b = val, .length = 8 * sizeof(uchar)});
}

bool buff_writeInt(OutputFileBuffer buff, int val){
    return buff_writeBits(buff, (Bits){.b = val, .length = 8 * sizeof(int)});
}

bool buff_flush(OutputFileBuffer buff){
    if(buff.bits->length == 0) { return true; }
    int remaining = 8 - buff.bits->length;
    
    return buff_writeBits(buff, (Bits){
        .b = 0,
        .length = remaining
    });
}

//TODO read from
// ret NULLBIT if EOF or ERR
Bits buff_readBit(InputFileBuffer buff){
    uchar data = '\0';
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

Bits buff_readBits(InputFileBuffer buff, int bitCount){
    Bits b = { .b = 0, .length = 0 };

    int bitsRead = 0;

    while(bitsRead < bitCount){
        Bits bit = buff_readBit(buff);
        if(isNullbit(bit)){ return NULLBIT; }
        
        bits_pushBit(&b, bit);

        ++bitsRead;
    }

    return b;
}

//We need to keep this bits for NULLBIT / EOF to work
Bits buff_readChar(InputFileBuffer buff){
    return buff_readBits(buff, 8);
}

Bits buff_readInt(InputFileBuffer buff){
    return buff_readBits(buff, sizeof(int)*8);
}

#ifndef DECODER_H
#define DECODER_H

typedef enum {
    HEADER,
    DISASSEMBLE,
    PRG,
    CHR
} option;

void dec_init(const char* filename);

void dec_exit();

void decode(option opt);

#endif


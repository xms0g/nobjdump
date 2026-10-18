#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

typedef enum {
    UNDEF, IMP, ACC, IMM, ABS,
    X_IN_ABS, Y_IN_ABS,
    ABS_IND, ZP,
    X_IN_ZP, Y_IN_ZP,
    X_IN_ZP_IND, ZP_IND_Y_IN,
    REL
} addressing_mode;

typedef struct {
    const char* format;
    addressing_mode a_mode;
    uint32_t operand_count;
} mnemonic;

void op_init_table();

mnemonic* op_find_mnemonic(uint8_t opcode);

#endif

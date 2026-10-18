#include "decoder.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opcode.h"

#define MAGIC0 0x4E
#define MAGIC1 0x45
#define MAGIC2 0x53
#define MAGIC3 0x1A

#define PRG_rom_SIZE 16384
#define PRG_rom_ADDRESS 0x8000
#define CHR_rom_SIZE 8192
#define BYTES_PER_ROW 16
#define INES_HEADER_SIZE 16
#define BUFFER_SIZE 32

#define error(msg) do { fprintf(stderr, "%s", msg); fclose(rom_file); exit(EXIT_FAILURE); } while (0)

typedef struct {
    uint16_t address;
    uint32_t size;
    uint8_t* data;
} rom;

static rom prg_rom;
static rom chr_rom;
static uint8_t header[INES_HEADER_SIZE];

static void display_header();
static void display_rom(const rom* rom);
static void display_formatted(const uint8_t* data, size_t len, uint16_t index);
static void disassemble();

void dec_init(const char* filename) {
    FILE* rom_file = fopen(filename, "rb");

    fread(header, sizeof(uint8_t), INES_HEADER_SIZE, rom_file);

    if (header[0] != MAGIC0 || header[1] != MAGIC1 ||
        header[2] != MAGIC2 || header[3] != MAGIC3) {
        error("Invalid NES rom file\n");
    }

    prg_rom.size = PRG_rom_SIZE * header[4];
    if (!prg_rom.size) {
        error("No PRG rom\n");
    }

    prg_rom.data = malloc(prg_rom.size);
    prg_rom.address = PRG_rom_ADDRESS;

    chr_rom.size = CHR_rom_SIZE * header[5];
    chr_rom.data = malloc(chr_rom.size);
    chr_rom.address = 0;

    fseek(rom_file, INES_HEADER_SIZE, SEEK_SET);
    fread(prg_rom.data, sizeof(uint8_t), prg_rom.size, rom_file);

    fseek(rom_file, INES_HEADER_SIZE + prg_rom.size, SEEK_SET);
    fread(chr_rom.data, sizeof(uint8_t), chr_rom.size, rom_file);

    fclose(rom_file);
}

void dec_exit() {
    free(prg_rom.data);
    free(chr_rom.data);
}

void decode(const option opt) {
    switch (opt) {
        case HEADER:
            display_header();
            break;
        case DISASSEMBLE:
            disassemble();
            break;
        case PRG:
            display_rom(&prg_rom);
            break;
        case CHR:
            display_rom(&chr_rom);
            break;
    }
}

static void display_header() {
    printf("iNES Header:\n"
           "    Magic Number: \t%02X %02X %02X %02X\n"
           "    Size of PRG rom: \t%d KB (%d count of 16 KB units)\n"
           "    Size of CHR rom: \t%d KB (%d count of 8 KB units)\n"
           "    Flags 6: \t\t0x%02x\n"
           "    Flags 7: \t\t0x%02x\n"
           "    Flags 8: \t\t0x%02x\n"
           "    Flags 9: \t\t0x%02x\n"
           "    Flags 10:\t\t0x%02x\n"
           "    Unused padding:\t0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
           header[0], header[1], header[2], header[3], prg_rom.size / 1024, header[4],
           chr_rom.size / 1024, header[5], header[6], header[7], header[8], header[9],
           header[10], header[11], header[12], header[13], header[14], header[15]);
}

static void display_rom(const rom* rom) {
    display_formatted(rom->data, rom->size, rom->address);
}

static void display_formatted(const uint8_t* data, const size_t len, const uint16_t index) {
    for (size_t i = 0; i < len; i += BYTES_PER_ROW) {
        char ascii[BYTES_PER_ROW + 1] = {0};

        printf("%06X:\t", index + (uint16_t) i);

        for (size_t j = 0; j < BYTES_PER_ROW; ++j) {
            const uint8_t byte = data[i + j];
            printf("%02X ", byte);
            ascii[j] = isprint(byte) ? byte : '.';
        }

        ascii[BYTES_PER_ROW] = '\0';
        printf(" %s\n", ascii);
    }
}

static void disassemble() {
    for (uint32_t i = 0; i < prg_rom.size; ++i) {
        const uint8_t* rom_data = &prg_rom.data[i];
        const uint8_t opcode = *rom_data;
        const uint32_t current_addr = prg_rom.address;
        const mnemonic* mnem = op_find_mnemonic(opcode);

        if (mnem->a_mode != IMP && mnem->a_mode != ACC && mnem->a_mode != UNDEF) {
            i += mnem->operand_count;
            uint8_t operands[2] = {0};

            for (int j = 1; j <= mnem->operand_count; ++j) {
                operands[j - 1] = *(rom_data + j);
            }

            char byte_opr[BUFFER_SIZE] = "";
            char mnem_opr[BUFFER_SIZE] = "";
            char temp[BUFFER_SIZE] = "";
            for (int j = 0; j < mnem->operand_count; ++j) {
                snprintf(temp, sizeof(temp), "%02X ", operands[j]);
                strncat(byte_opr, temp, sizeof(byte_opr) - strlen(byte_opr) - 1);
            }

            if (mnem->a_mode == REL) {
                const int8_t rel_offset = (int8_t) operands[0];
                const uint16_t rel_addr = current_addr + mnem->operand_count + 1 + rel_offset;
                snprintf(mnem_opr, sizeof(mnem_opr), "%04X", rel_addr);
            } else if (mnem->operand_count == 2) {
                snprintf(mnem_opr, sizeof(mnem_opr), "%02X%02X", operands[1], operands[0]); // little-endian
            } else if (mnem->operand_count == 1) {
                snprintf(mnem_opr, sizeof(mnem_opr), "%02X", operands[0]);
            }

            char out[BUFFER_SIZE] = "";
            snprintf(out, sizeof(out), mnem->format, mnem_opr);
            printf("%06X:\t%02X %s%s%s\n", prg_rom.address, opcode, byte_opr,
                   mnem->operand_count == 2 ? "\t" : "\t\t", out);
        } else {
            printf("%06X:\t%02X\t\t%s\n", prg_rom.address, opcode, mnem->format);
        }
        prg_rom.address += mnem->operand_count + 1;
    }
}

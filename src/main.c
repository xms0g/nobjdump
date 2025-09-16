#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decoder.h"
#include "opcode.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 3
#define VERSION_PATCH 0

#define STRINGIFY0(s) # s
#define STRINGIFY(s) STRINGIFY0(s)
#define VERSION STRINGIFY(VERSION_MAJOR) "." STRINGIFY(VERSION_MINOR) "." STRINGIFY(VERSION_PATCH)

int main(int argc, char** argv) {
    static const char* usage = "OVERVIEW: NES ROM image dumper\n\n"
                               "USAGE: nobjdump [options] <input rom image>\n\n"
                               "OPTIONS:\n"
                               "  -h, --header          Display iNES header\n"
                               "  -d, --disassemble     Display the assembler mnemonics for the machine instructions\n"
                               "  -c, --chr             Display CHR ROM data\n"
                               "  -p, --prg             Display PRG ROM data\n"
                               "  -H  --help            Display available options\n"
                               "  -v, --version         Display the version of this program\n";
    if (argc < 3) {
        if (argc == 2 && (!strcmp(argv[1], "-H") || !strcmp(argv[1], "--help"))) {
            printf("%s\n", usage);
        } else if (argc == 2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) {
            printf("nobjdump version %s\n", VERSION);
        } else {
            fprintf(stderr, "%s\n", usage);
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    Option opt;

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--header")) {
        opt = HEADER;
    } else if (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--disassemble")) {
        opt = DISASSEMBLE;
    } else if (!strcmp(argv[1], "-c") || !strcmp(argv[1], "--chr")) {
        opt = CHR;
    } else if (!strcmp(argv[1], "-p") || !strcmp(argv[1], "--prg")) {
        opt = PRG;
    } else {
        fprintf(stderr, "%s\n", usage);
        return EXIT_FAILURE;
    }

    opInitTable();
    decInit(argv[2]);
    decode(opt);
    decExit();

    return EXIT_SUCCESS;
}

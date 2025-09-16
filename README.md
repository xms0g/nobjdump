# nobjdump
A lightweight command-line utility for NES, inspired by GNU objdump, designed to inspect NES ROMs.
## Features
+ `NES-specific insights:` Extract and display headers, and PRG/CHR memory layout.
+ `Disassembly:` Translate the 6502 opcode to the related mnemonics.
## Compatibility
+ Works on Linux, Windows, and MacOS systems.
+ Supports standard `.nes` files that conform to the iNES format.
## Usage
```bash
➜  ~ nobjdump -h
OVERVIEW: NES ROM image dumper

USAGE: nobjdump [options] <input rom image>

OPTIONS:
  -h, --header          Display iNES header
  -d, --disassemble     Display the assembler mnemonics for the machine instructions
  -c, --chr             Display CHR ROM data
  -p, --prg             Display PRG ROM data
  -H  --help            Display available options
  -v, --version         Display the version of this program
```
## Example
```asm
➜  ~ nobjdump -h sunset.nes
iNES Header:
    Magic Number: 	4E 45 53 1A
    Size of PRG ROM: 	32768 (2 count of 16 KB units)
    Size of CHR ROM: 	8192 (1 count of 8 KB units)
    Flags 6: 		0x0
    Flags 7: 		0x0
    Flags 8: 		0x0
    Flags 9: 		0x0
    Flags 10:		0x0
    Unused padding:	0x0
```
```asm
➜  ~ nobjdump -p sunset.nes
000010:	40 48 8A 48 98 48 A9 00 8D 03 20 A9 02 8D 14 40  @H.H.H.... ....@
000020:	A9 00 8D 05 20 8D 05 20 E6 13 A5 13 C9 0F F0 03  .... .. ........
000030:	4C 30 80 A9 00 85 13 20 F3 80 A5 12 49 01 85 12  L0..... ....I...
000040:	A9 90 8D 00 20 A9 1E 8D 01 20 68 A8 68 AA 68 40  .... .... h.h.h@
000050:	A2 00 95 00 9D 00 01 9D 00 03 9D 00 04 9D 00 05  ................
000060:	9D 00 06 9D 00 07 A9 FF 9D 00 02 A9 00 E8 D0 E2  ................
000070:	A9 00 85 12 85 13 85 14 20 86 80 20 CA 80 20 AF  ........ .. .. .
000080:	80 20 A1 80 2C 02 20 10 FB A9 90 8D 00 20 A9 1E  . ..,. ...... ..
000090:	8D 01 20 4C 83 80 AE 02 20 A2 3F 8E 06 20 A2 00  .. L.... .?.. ..
0000A0:	8E 06 20 A2 00 BD 00 88 8D 07 20 E8 E0 20 D0 F5  .. ....... .. ..
0000B0:	60 A2 00 BD 25 81 9D 00 02 E8 E0 E0 D0 F5 60 AD  `...%.........`.
0000C0:	02 20 A9 23 8D 06 20 A9 CA 8D 06 20 A2 00 BD 20  . .#.. .... ... 
0000D0:	88 8D 07 20 E8 E0 40 D0 F5 60 AD 02 20 A9 20 8D  ... ..@..`.. . .
```
```asm
➜  ~ nobjdump -d sunset.nes
008000:	40		    RTI
008001:	48		    PHA
008002:	8A		    TXA
008003:	48		    PHA
008004:	98		    TYA
008005:	48		    PHA
008006:	A9 00 		LDA #$00
008008:	8D 03 20 	STA $2003
00800B:	A9 02 		LDA #$02
00800D:	8D 14 40 	STA $4014
008010:	A9 00 		LDA #$00
008012:	8D 05 20 	STA $2005
008015:	8D 05 20 	STA $2005
008018:	E6 13 		INC $13
00001E:	A5 13 		LDA $13
```

## License
This project is licensed under the MIT License. See the LICENSE file for details.



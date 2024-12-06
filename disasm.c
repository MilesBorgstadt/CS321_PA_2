#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    FILE *file;
    file = fopen(argv[1], "rb");

    char *output;
    int instruction;
    
    
    while (!feof(file)) {
        fread(&instruction, 4, 1, file);
        instruction = be32toh(instruction);

        char line[21];
        bool branch = getType(line, instruction);
        output = realloc(output, sizeof(output) + strlen(line));
        output = strcat(output, line);

    }
}

bool getType(char *line, int instruction) {
    int Rm = (instruction >> 16) & 0x1F;
    int shamt = (instruction >> 10) & 0x3F;
    int Rn = (instruction >> 5) & 0x1F;
    int Rd = instruction & 0x1F;
    int ALUImm = (instruction >> 10) & 0xFFF;
    int DTAdd = (instruction >> 12) & 0x1FF;
    int op = (instruction >> 10) & 0x3;
    int Rt = instruction & 0x1F;
    int BAdd = instruction & 0x3FFFFFF;
    int CBAdd = (instruction >> 5) & 0x7FFFF;

    // switch for 6-bit opcodes
    int opcode = (instruction >> 26) & 0x3F;
    switch(opcode) {
        case 0b000101:
            sprintf(line, "B %d", BAdd);
            return true;
        case 0b100101:
            sprintf(line, "BL %d", BAdd);
            return true;
    }

    // switch for 8-bit opcodes
    opcode = (instruction >> 24) & 0xFF;
    switch(opcode) {
        case 0b10110101:     // CBNZ
        case 0b10110100:     // CBZ
        case 0b01010100:     // B.cond
            return true;   
    }

    // switch for 10-bit opcodes
    opcode = (instruction >> 22) & 0x3FF;
    switch(opcode) {
        case 0b1101000100:   // SUBI
        case 0b1111000100:   // SUBIS
        case 0b1101001000:   // EORI
        case 0b1011001000:   // ORRI
        case 0b1001000100:   // ADDI
        case 0b1001001000:   // ANDI
            return false;
    }

    // switch for 11-bit opcodes
    opcode = (instruction >> 21) & 0x7FF;
    switch(opcode) {
        case 0b10001011000:  // ADD
        case 0b10001010000:  // AND
        case 0b11010110000:  // BR
        case 0b11001010000:  // EOR
        case 0b11111000010:  // LDUR
        case 0b11010011011:  // LSL
        case 0b11010011010:  // LSR
        case 0b10101010000:  // ORR
        case 0b11111000000:  // STUR
        case 0b11001011000:  // SUB
        case 0b11101011000:  // SUBS
        case 0b11111111101:  // PRNT
        case 0b11111111100:  // PRNL
        case 0b11111111110:  // DUMP
        case 0b11111111111:  // HALT
            return false;
    }

    return false;
}
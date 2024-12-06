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
        case 0b000101:      // B
            sprintf(line, "B %d", BAdd);
            return true;
        case 0b100101:      // BL
            sprintf(line, "BL %d", BAdd);
            return true;
    }

    // switch for 8-bit opcodes
    opcode = (instruction >> 24) & 0xFF;
    switch(opcode) {
        case 0b10110101:     // CBNZ
            sprintf(line, "CBNZ X%d, %d", Rt, CBAdd);
            return true;
        case 0b10110100:     // CBZ
            sprintf(line, "CBZ X%d, %d", Rt, CBAdd);
            return true;
        case 0b01010100:     // B.cond
            int cond = instruction & 0x1F;
            switch(cond) {
                case 0:
                    sprintf(line, "B.EQ %d", CBAdd);
                case 1:
                    sprintf(line, "B.NE %d", CBAdd);
                case 2:
                    sprintf(line, "B.HS %d", CBAdd);
                case 3:
                    sprintf(line, "B.LO %d", CBAdd);
                case 4:
                    sprintf(line, "B.MI %d", CBAdd);
                case 5:
                    sprintf(line, "B.PL %d", CBAdd);
                case 6:
                    sprintf(line, "B.VS %d", CBAdd);
                case 7:
                    sprintf(line, "B.VC %d", CBAdd);
                case 8:
                    sprintf(line, "B.HI %d", CBAdd);
                case 9:
                    sprintf(line, "B.LS %d", CBAdd);
                case 10:
                    sprintf(line, "B.GE %d", CBAdd);
                case 11:
                    sprintf(line, "B.LT %d", CBAdd);
                case 12:
                    sprintf(line, "B.GT %d", CBAdd);
                case 13:
                    sprintf(line, "B.LE %d", CBAdd);
            }
            return true;   
    }

    // switch for 10-bit opcodes
    opcode = (instruction >> 22) & 0x3FF;
    switch(opcode) {
        case 0b1101000100:   // SUBI
            sprintf(line, "SUBI X%d, X%d, #%d", Rd, Rn, ALUImm);
            return false;
        case 0b1111000100:   // SUBIS
            sprintf(line, "SUBIS X%d, X%d, #%d", Rd, Rn, ALUImm);
            return false;
        case 0b1101001000:   // EORI
            sprintf(line, "EORI X%d, X%d, #%d", Rd, Rn, ALUImm);
            return false;
        case 0b1011001000:   // ORRI
            sprintf(line, "ORRI X%d, X%d, #%d", Rd, Rn, ALUImm);
            return false;
        case 0b1001000100:   // ADDI
            sprintf(line, "ADDI X%d, X%d, #%d", Rd, Rn, ALUImm);
            return false;
        case 0b1001001000:   // ANDI
            sprintf(line, "ANDI X%d, X%d, #%d", Rd, Rn, ALUImm);
            return false;
    }

    // switch for 11-bit opcodes
    opcode = (instruction >> 21) & 0x7FF;
    switch(opcode) {
        case 0b10001011000:  // ADD
            sprintf(line, "ADD %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b10001010000:  // AND
            sprintf(line, "AND %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b11010110000:  // BR
            sprintf(line, "BR %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b11001010000:  // EOR
            sprintf(line, "EOR %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b11111000010:  // LDUR
            sprintf(line, "LDUR %d", DTAdd, op, Rn, Rt);
            return false;
        case 0b11010011011:  // LSL
            sprintf(line, "LSL %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b11010011010:  // LSR
            sprintf(line, "LSR %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b10101010000:  // ORR
            sprintf(line, "ORR %d", Rm, shamt, Rn, Rd);
            return false;
        case 0b11111000000:  // STUR
            sprintf(line, "STUR %d", DTAdd, op, Rn, Rt);
            return false;
        case 0b11001011000:  // SUB
            sprintf(line, "SUB X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b11101011000:  // SUBS
            sprintf(line, "SUBS X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b11111111101:  // PRNT
            sprintf(line, "PRNT X%d", Rd);
            return false;
        case 0b11111111100:  // PRNL
            sprintf(line, "PRNL");
            return false;
        case 0b11111111110:  // DUMP
            sprintf(line, "SUB");
            return false;
        case 0b11111111111:  // HALT
            sprintf(line, "HALT");
            return false;
    }

    return false;
}
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
    int opcode = (instruction >> 26) & 0x3F;
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

    //      B.cond: This is a CB instruction in which the Rt field is not a register, but
    //         a code that indicates the condition extension. These have the values
    //         (base 16):
    //         0: EQ
    //         1: NE
    //         2: HS
    //         3: LO
    //         4: MI
    //         5: PL
    //         6: VS
    //         7: VC
    //         8: HI
    //         9: LS
    //         a: GE
    //         b: LT
    //         c: GT
    //         d: LE
    // CBNZ 0b10110101
    // CBZ 0b10110100
    // EOR 0b11001010000
    // EORI 0b1101001000
    // LDUR 0b11111000010
    // LSL: This instruction uses the shamt field to encode the shift amount, while
    //      Rm is unused. 0b11010011011
    // LSR: Same as LSL. 0b11010011010
    // ORR 0b10101010000
    // ORRI 0b1011001000
    // STUR 0b11111000000
    // SUB 0b11001011000
    // SUBI 0b1101000100
    // SUBIS 0b1111000100
    // SUBS 0b11101011000
    // MUL
    // PRNT: This is an added instruction (part of our emulator, but not part of LEG
    //       or ARM) that prints a register name and its contents in hex and
    //       decimal.  This is an R instruction.  The opcode is 11111111101.  The
    //       register is given in the Rd field.
    // PRNL: This is an added instruction that prints a blank line.  This is an R
    //       instruction.  The opcode is 11111111100.
    // DUMP: This is an added instruction that displays the contents of all
    //       registers and memory, as well as the disassembled program.  This is an
    //       R instruction.  The opcode is 11111111110.
    // HALT: This is an added instruction that triggers a DUMP and terminates the
    //       emulator.  This is an R instruction.  The opcode is 11111111111
    int R_TYPE_OPCODES[] = {
        0b10001011000, //ADD
        0b10001010000 //AND
        
    }

    I_TYPE_OPCODES = {
        0b1001000100, //ADDI
        0b10010010000, //ANDI

    }

    B_TYPE_OPCODES = {
        0b000101, //B
        0b100101, //BL
        0b11010110000 //BR: The branch target is encoded in the Rn field.
    }
}
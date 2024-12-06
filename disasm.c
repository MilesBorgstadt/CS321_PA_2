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

    switch(opcode) {
        case 0b10001011000: //ADD
            break;
        case 0b1001000100: //ADDI
            break;
        case 0b10001010000
        //etc... im adding those
        default:
            break;
    }
}
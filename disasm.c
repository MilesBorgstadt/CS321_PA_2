#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    file = fopen(argv[1], "rb");

    int size = 0;
    char *output;
    int instruction;
    
    while (!feof(file)) {

    }
}

void getType(int instruction) {
    int opcode = (instruction >> 26) & 0x3F;

    switch (opcode) {
        case 0b110101:  //R-type
            parseR(instruction);
            break;
        case 0b010101:  //I-type
            parseI(instruction);
            break;
        case 0b000001:  //D-type
            parseD(instruction);
            break;
        case 0b101010:  //B-type
            parseB(instruction);
            break;
        case 0b110001:  //CB-type
            parseCB(instruction);
            break;
        default:
            printf("opcode %06x instruction type unknown\n", opcode);
            break;
    }
}

void parseR(int instruction);
void parseI(int instruction);
void parseD(int instruction);
void parseB(int instruction);
void parseCB(int instruction);
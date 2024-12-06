#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    file = fopen(argv[1], "rb");

    char *output;
    int instruction;

    while (!feof(file)) {
        fread(&instruction, 4, 1, file);
        instruction = be32toh(instruction);
        output = realloc(output, sizeof(output) + 4);
        getType(instruction);
    }
}
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LABEL_SIZE 100
#define MAX_LABELS 100

typedef struct labelInfo {
    char name[MAX_LABEL_SIZE];
    int lineNumber;
} Label;

bool getType(char *line, int instruction, int *branchAddress) {
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


    // Check if the BAdd is negative
    if (((BAdd >> 25) & 0x1)) {
			BAdd = -(((BAdd ^ 0xFFFFFF) + 1) & 0xFFFFFF);
	}
       if (((CBAdd >> 25) & 0x1)) {
			CBAdd = -(((CBAdd ^ 0xFFFFFF) + 1) & 0xFFFFFF);
	}

    // switch for 6-bit opcodes
    int opcode = (instruction >> 26) & 0x3F;
    switch(opcode) {
        case 0b000101:      // B
            sprintf(line, "B %d", BAdd);
            *branchAddress = BAdd;  // Save the BAdd value
            return true;
        case 0b100101:      // BL
            sprintf(line, "BL %d", BAdd);
            *branchAddress = BAdd;  // Save the BAdd value
            return true;
    }

    // switch for 8-bit opcodes
    opcode = (instruction >> 24) & 0xFF;
    switch(opcode) {
        case 0b10110101:     // CBNZ
            sprintf(line, "CBNZ X%d, %d", Rt, CBAdd);
            *branchAddress = CBAdd;  // Save the CBAdd value
            return true;
        case 0b10110100:     // CBZ
            sprintf(line, "CBZ X%d, %d", Rt, CBAdd);
            *branchAddress = CBAdd;  // Save the CBAdd value
            return true;
        case 0b01010100:     // B.cond
            int cond = instruction & 0x1F;
            switch(cond) {
                case 0:
                    sprintf(line, "B.EQ %d",  CBAdd);
                case 1:
                    sprintf(line, "B.NE %d", CBAdd);
                case 2:
                    sprintf(line, "B.HS %d",  CBAdd);
                case 3:
                    sprintf(line, "B.LO %d",  CBAdd);
                case 4:
                    sprintf(line, "B.MI %d",  CBAdd);
                case 5:
                    sprintf(line, "B.PL %d",  CBAdd);
                case 6:
                    sprintf(line, "B.VS %d",  CBAdd);
                case 7:
                    sprintf(line, "B.VC %d",  CBAdd);
                case 8:
                    sprintf(line, "B.HI %d",  CBAdd);
                case 9:
                    sprintf(line, "B.LS %d",  CBAdd);
                case 10:
                    sprintf(line, "B.GE %d",  CBAdd);
                case 11:
                    sprintf(line, "B.LT %d",  CBAdd);
                case 12:
                    sprintf(line, "B.GT %d",  CBAdd);
                case 13:
                    sprintf(line, "B.LE %d",  CBAdd);
            }
            *branchAddress = CBAdd;  // Save the CBAdd value
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
            sprintf(line, "ADD X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b10001010000:  // AND
            sprintf(line, "AND X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b11010110000:  // BR
            if (Rd == 30) {
                sprintf(line, "BR LR");
            } else {
                sprintf(line, "BR X%d", Rd);
            }
            return false;
        case 0b11001010000:  // EOR
            sprintf(line, "EOR X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b11111000010:  // LDUR
            sprintf(line, "LDUR X%d, [X%d, #%d]", Rt, Rn, DTAdd);
            return false;
        case 0b11010011011:  // LSL
            sprintf(line, "LSL X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b11010011010:  // LSR
            sprintf(line, "LSR X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b10101010000:  // ORR
            sprintf(line, "ORR X%d, X%d, X%d", Rd, Rn, Rm);
            return false;
        case 0b11111000000:  // STUR
            sprintf(line, "STUR X%d, [X%d, #%d]", Rt, Rn, DTAdd);
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
            sprintf(line, "DUMP");
            return false;
        case 0b11111111111:  // HALT
            sprintf(line, "HALT");
            return false;
    }

    return false;
}

int main(int argc, char *argv[]) {
    FILE *file;
    file = fopen(argv[1], "rb");

    if (file == NULL) {
        printf("File not found\n");
        return 1;
    }
    
    int instruction;
    
    //Keep track of the line number
    int lineNumber = 1;

    //Keep track of label line numbers
    Label *labels;
    labels = malloc(sizeof(Label) * MAX_LABELS);

    int i = 0;
    int numLabels = 0;
    //Find where the labels are at
    while (!feof(file)) {
        fread(&instruction, 4, 1, file);
        instruction = be32toh(instruction);

        int branchTargetLine = -1;  // Store the branch target address
        char dummyLine[32];
        if (getType(dummyLine, instruction, &branchTargetLine)) {
            // Store what line the branch target is on
            char name[MAX_LABEL_SIZE];
            sprintf(name, "label_%d", i);
            Label newLabel; 
            strcpy(newLabel.name, name);
            newLabel.lineNumber = lineNumber + branchTargetLine;

            // Add the label to the list
            labels[i++] = newLabel;
            numLabels++;

        }
        lineNumber++;
    }

    // Rewind the file and start counting lines
    rewind(file);
    lineNumber = 1;

    int labelIndex = 0;

    
    while (!feof(file)) {
        fread(&instruction, 4, 1, file);
        instruction = be32toh(instruction);


        //Check if there is a label at this line
        for (int j = 0; j < numLabels; j++) {
            if (labels[j].lineNumber == lineNumber) {
                printf("%s:\n", labels[j].name);
                break;
            }
        }

        char line[21];
        int branchAddress;

        if(getType(line, instruction, &branchAddress)){
            char newLine[21]; 
            sscanf(line, "%s ", &newLine);
            printf("\t%s %s \n", newLine, labels[labelIndex++].name);
        } else {
            printf("\t%s\n", line);
        }

        // Just change to print the line
        // output = realloc(output, sizeof(output) + strlen(line));
        // output = strcat(output, line);
        
        lineNumber++;
    }

    // Free the memory #FreeMyBoyLabels
    free(labels);
    fclose(file);
}
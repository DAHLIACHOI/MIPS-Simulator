#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#define MEMORY_SIZE 0x100000

unsigned char progMEM[MEMORY_SIZE], dataMEM[MEMORY_SIZE], stackMEM[MEMORY_SIZE];

int MEM(unsigned int A, int V, int nRW, int S) {
    unsigned int memSelect = A >> 20;
    unsigned int offset = A & 0xFFFFF;
    unsigned char* pM;

    if (memSelect == 0x004) pM = progMEM;   
    else if (memSelect == 0x100) pM = dataMEM; 
    else if (memSelect == 0x7FF) pM = stackMEM;  
    else {return -1; }

    int size = 0;
    if (S == BYTE) size = 1;
    else if (S == HALF) size = 2;
    else if (S == WORD) size = 4;

    if (nRW == 0) { 
        int word = 0;

        for (int i = 0; i < size; i++)
            ((unsigned char*)&word)[size - i - 1] = pM[offset + i];

        return word;
    }
    else if (nRW == WR) { 
        for (int i = 0; i < size; i++)
            pM[offset + (size - i - 1)] = ((unsigned char*)&V)[i];
    }

    return 0;
}

void viewMemory(unsigned int start, unsigned int end) {
    printf("------------------------\n");
    printf("|  address |   value   |\n");
    printf("|----------------------|\n");
    for (int address = start; address <= end; address += 4) {
        int word = MEM(address, 0, RD, WORD);
        printf("| %8x | %9x |\n", address, word);
    }
    printf("------------------------\n");
}

void resetMemory(void)
{
    int i;
    for (i = 0; i < MEMORY_SIZE; i++) {
        progMEM[i] = 0;
        dataMEM[i] = 0;
        stackMEM[i] = 0;
    }
}

void setMemory(unsigned int address, int value) {
    MEM(address, value, WR, WORD);
}

#include <stdio.h>
#include "defines.h"


unsigned int REGISTERS[REGISTER_SIZE];
unsigned int* R = REGISTERS;

int PC = 0x400000;
int HI = 0;
int LO = 0;


unsigned int REG(unsigned int A, unsigned int V, unsigned int nRW) {
    if (nRW == RD) return REGISTERS[A];
    else if (nRW == WR) REGISTERS[A] = V;
    return 0;
}


void viewRegister() {
    printf("------------------------\n");
    printf("| register |   value   |\n");
    printf("|----------------------|\n");
    printf("| %8s | %9x |\n", "PC", PC);
    printf("| %8s | %9x |\n", "HI", HI);
    printf("| %8s | %9x |\n", "LO", LO);
    printf("|          |           |\n");
    for (int i = 0; i < REGISTER_SIZE; i++) {
        char* name = REGISTER_NAME[i];
        unsigned int value = REG(i, 0, RD);
        printf("| %8s | %9x |\n", name, value);
    }
    printf("------------------------\n");
}

void resetRegister() {
    for (int i = 0; i < REGISTER_SIZE; i++) {
        REG(i, 0, WR);
    }

    HI = 0;
    LO = 0;
    REG(29, 0x80000000, WR);
}

void setRegister(unsigned int number, int value) {
    REG(number, value, WR);
}


void jumpProgram(unsigned int address) {
    PC = address;
}

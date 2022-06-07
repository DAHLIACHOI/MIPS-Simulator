#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"


void instructionToString(unsigned int word, char** str) {
    union instructionRegister instruction = { word };

    unsigned int opcode = instruction.R.opc;
    unsigned int funct = instruction.R.fct;

    unsigned int opc1 = opcode >> 3;
    unsigned int opc2 = (opcode << 29) >> 29;

    unsigned int funct1 = (funct << 26) >> 29;
    unsigned int funct2 = (funct << 29) >> 29;

    char* op = opcode ? OPCODE[opc1][opc2] : FUNCT[funct1][funct2];
    char* rs = REGISTER_NAME[instruction.R.rs];
    char* rt = REGISTER_NAME[instruction.R.rt];
    char* rd = REGISTER_NAME[instruction.R.rd];
    unsigned int sh = instruction.R.sht;
    unsigned int target = instruction.J.jval;
    unsigned int u_immediate = instruction.I.offset;
    int immediate = (int)instruction.I.offset; 


    if (opcode == 0) {
        switch (funct) {
        case 0: case 2: case 3:
            sprintf(*str, "%s %s, %s, %d", op, rd, rt, sh);
            break;
        case 8:
            sprintf(*str, "%s %s", op, rs);
            break;
        case 12:
            sprintf(*str, "syscall");
            break;
        case 16: case 18:
            sprintf(*str, "%s %s", op, rd);
            break;
        default:
            sprintf(*str, "%s %s, %s, %s", op, rd, rs, rt);
        }
    }
    else {
        switch (opcode) {
        case 2: case 3:
            sprintf( *str, "%s %x", op, target);
            break;
        case 1:
            sprintf(*str, "%s %s, %d", op, rs, immediate);
            break;
        case 4: case 5:
            sprintf(*str, "%s %s, %s, %d", op, rs, rt, immediate);
            break;
        case 8: case 10:
            sprintf(*str, "%s %s, %s, %d", op, rt, rs, immediate);
            break;
        case 12: case 13: case 14:
            sprintf(*str, "%s %s, %s, %d", op, rt, rs, u_immediate);
            break;
        case 15:
            sprintf(*str, "%s %s, %d", op, rt, u_immediate);
            break;
        case 35 :case 43: case 32: case 40: case 36:
            sprintf(*str, "%s %s, %d(%s)", op, rt, immediate, rs);
            break;
        default:
            sprintf(*str, "잘못된 명령어입니다.: %x", instruction.i);
        }
    }
}

void printInstruction(unsigned int word) {
    char* str = (char*)malloc((100) * sizeof(char));
    instructionToString(word, &str);
    printf("%s\n", str);
    free(str);
}

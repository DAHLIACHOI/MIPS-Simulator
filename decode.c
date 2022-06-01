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
    char* rs = REGISTER_STR[instruction.R.rs];
    char* rt = REGISTER_STR[instruction.R.rt];
    char* rd = REGISTER_STR[instruction.R.rd];
    unsigned int sh = instruction.R.sht;
    unsigned int target = instruction.J.jval;
    unsigned int u_immediate = instruction.I.offset;
    int immediate = (int)instruction.I.offset; 


    if (opcode == R_FORMAT) {
        switch (funct) {
        case SLL: case SRL: case SRA:
            sprintf(*str, "%s %s, %s, %d", op, rd, rt, sh);
            break;
        case JR:
            sprintf(*str, "%s %s", op, rs);
            break;
        case SYSCALL:
            sprintf(*str, "syscall");
            break;
        case MFHI: case MFLO:
            sprintf(*str, "%s %s", op, rd);
            break;
        default:
            sprintf(*str, "%s %s, %s, %s", op, rd, rs, rt);
        }
    }
    else {
        switch (opcode) {
        case J: case JAL:
            sprintf( *str, "%s %x", op, target);
            break;
        case BLTZ:
            sprintf(*str, "%s %s, %d", op, rs, immediate);
            break;
        case BEQ: case BNE:
            sprintf(*str, "%s %s, %s, %d", op, rs, rt, immediate);
            break;
        case ADDI: case ADDIU: case SLTI: case SLTIU:
            sprintf(*str, "%s %s, %s, %d", op, rt, rs, immediate);
            break;
        case ANDI: case ORI: case XORI:
            sprintf(*str, "%s %s, %s, %d", op, rt, rs, u_immediate);
            break;
        case LUI:
            sprintf(*str, "%s %s, %d", op, rt, u_immediate);
            break;
        case LW: case SW: case LB: case SB: case LBU:
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

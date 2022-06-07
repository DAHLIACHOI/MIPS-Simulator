#include "defines.h"
#include "stddef.h"

extern int PC;
extern int HI;
extern int LO;
extern unsigned int* R;

int sll(int rd, int rt, int sh) {
    R[rd] = ALU(R[rt], sh, 1, NULL);
    return 0;
}

int srl(int rd, int rt, int sh) {
    R[rd] = ALU(R[rt], sh, 2, NULL);
    return 0;
}

int sra(int rd, int rt, int sh) {
    R[rd] = ALU(R[rt], sh, 3, NULL);
    return 0;
}

int jr(int rs) {
    PC = R[rs];
    return 0;
}


int mfhi(int rd) {
    R[rd] = HI;
    return 0;
}

int mflo(int rd) {
    R[rd] = LO;
    return 0;
}

int mul(int rs, int rt) {
    long long int result = (long long int)R[rs] * (long long int)R[rt];
    HI = result >> 32;
    LO = (result << 32) >> 32;

    return 0;
}

int add(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 8, NULL);
    return 0;
}

int sub(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 9, NULL);
    return 0;
}

int and (int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 12, NULL);
    return 0;
}

int or (int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 13, NULL);
    return 0;
}

int xor (int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 14, NULL);
    return 0;
}

int nor(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 15, NULL);
    return 0;
}

int slt(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], 4, NULL);
    return 0;
}



int j(int address) {
    PC = ((PC + 4) & 0xf0000000) | (address << 2);
    return 0;
}

int jal(int address) {
    R[31] = PC;
    PC = ((PC + 4) & 0xf0000000) | (address << 2);
    return 0;
}


int bltz(int rs, int rt, int imm) {
    if (R[rs] < R[rt]) PC += imm * 4;
    return 0;
}

int beq(int rs, int rt, int imm) {
    if (R[rs] == R[rt]) PC += imm * 4;
    return 0;
}

int bne(int rs, int rt, int imm) {
    if (R[rs] != R[rt]) PC += imm * 4;
    return 0;
}

int addi(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, 8, NULL);
    return 0;
}

int slti(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, 4, NULL);
    return 0;
}

int andi(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, 12, NULL);
    return 0;
}


int ori(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, 13, NULL);
    return 0;
}

int xori(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, 14, NULL);
    return 0;
}

int lui(int rt, int imm) {
    R[rt] = (imm & 0xffff) << 16;
    return 0;
}

int lb(int rt, int imm, int rs) {
    R[rt] = MEM(R[rs] + imm, 0, RD, BYTE);
    return 0;
}

int lw(int rt, int imm, int rs) {
    R[rt] = MEM(R[rs] + imm, 0, RD, WORD);
    return 0;
}

int lbu(int rt, int imm, int rs) {
    R[rt] = MEM(R[rs] + imm, 0, RD, BYTE);
    return 0;
}

int sb(int rt, int imm, int rs) {
    MEM(R[rs] + imm, R[rt], WR, BYTE);
    return 0;
}

int sw(int rt, int imm, int rs) {
    MEM(imm + R[rs], R[rt], WR, WORD);
    return 0;
}

int syscall() {
    switch (R[2]) {
    case 1:  printf("%d\n", R[4]); break; // $a0에 있는 정수 출력해주기
    case 10: printf("\n"); return 1;
    }
    return 0;
}


union instructionRegister instructionFetch() {
    unsigned int word = MEM(PC, 0, RD, WORD);
    union instructionRegister instruction = { word };
    PC += 4;

    return instruction;
}

int stepProgram() {
    union instructionRegister instruction = instructionFetch();

    printInstruction(instruction.i);

    if (instruction.i == 0) return 1;

    unsigned int opcode = instruction.R.opc;
    unsigned int funct = instruction.R.fct;
    unsigned int rs = instruction.R.rs;
    unsigned int rt = instruction.R.rt;
    unsigned int rd = instruction.R.rd;
    unsigned int sh = instruction.R.sht;
    unsigned int address = instruction.J.jval;
    unsigned int u_imm = instruction.I.offset & 0xffff;
    int imm = instruction.I.offset; 

    if (opcode == 0) {
        switch (funct) {
        case 0:     return sll(rd, rt, sh);
        case 2:     return srl(rd, rt, sh);
        case 3:     return sra(rd, rt, sh);
        case 8:      return jr(rs);
        case 12: return syscall();
        case 16:    return mfhi(rd);
        case 18:    return mflo(rd);
        case 24:     return mul(rs, rt);
        case 32:     return add(rd, rs, rt);
        case 34:     return sub(rd, rs, rt);
        case 36:     return and (rd, rs, rt);
        case 37:      return or (rd, rs, rt);
        case 38:     return xor (rd, rs, rt);
        case 39:     return nor(rd, rs, rt);
        case 42:     return slt(rd, rs, rt);
        default:      printf("잘못된 명령어 입니다. \n");
        }
    }
    else {
        switch (opcode) {
        case 1: return bltz(rs, rt, imm);
        case 2:    return j(address);
        case 3:  return jal(address);
        case 4:  return beq(rs, rt, imm);
        case 5:  return bne(rs, rt, imm);
        case 8: return addi(rt, rs, imm);
        case 10: return slti(rt, rs, imm);
        case 12: return andi(rt, rs, u_imm);
        case 13:  return ori(rt, rs, u_imm);
        case 14: return xori(rt, rs, u_imm);
        case 15:  return lui(rt, u_imm);
        case 35:   return lw(rt, imm, rs);
        case 43:   return sw(rt, imm, rs);
        case 32:   return lb(rt, imm, rs);
        case 40:   return sb(rt, imm, rs);
        case 36:  return lbu(rt, imm, rs);
        default:   printf("잘못된 명령어 입니다.\n");
        }
    }

    return 1; 
}

void goProgram() {
    while (1) {
        int end = stepProgram();
        if (end) return;
    }
}

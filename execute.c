#include "defines.h"
#include "stddef.h"

enum ALU_OPS {
    ALU_NO_SHIFT, ALU_SLL, ALU_SRL, ALU_SRA, ALU_SET_LESS, ALU_ADD = 8, ALU_SUBTRACT, ALU_AND = 12, ALU_OR, ALU_XOR, ALU_NOR
};

extern int PC;
extern int HI;
extern int LO;
extern unsigned int* R;

int sll(int rd, int rt, int sh) {
    R[rd] = ALU(R[rt], sh, ALU_SLL, NULL);
    return 0;
}

int srl(int rd, int rt, int sh) {
    R[rd] = ALU(R[rt], sh, ALU_SRL, NULL);
    return 0;
}

int sra(int rd, int rt, int sh) {
    R[rd] = ALU(R[rt], sh, ALU_SRA, NULL);
    return 0;
}

int jr(int rs) {
    PC = R[rs];
    return 0;
}


int syscall() {
    switch (R[$v0]) {
    case 1:  printf("%d\n", R[$a0]); break;
    case 10: printf("\n"); return 1;
    case 11: printf("%c\n", R[$a0]); break;
    }
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
    R[rd] = ALU(R[rs], R[rt], ALU_ADD, NULL);
    return 0;
}

int sub(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], ALU_SUBTRACT, NULL);
    return 0;
}

int and (int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], ALU_AND, NULL);
    return 0;
}

int or (int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], ALU_OR, NULL);
    return 0;
}

int xor (int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], ALU_XOR, NULL);
    return 0;
}

int nor(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], ALU_NOR, NULL);
    return 0;
}

int slt(int rd, int rs, int rt) {
    R[rd] = ALU(R[rs], R[rt], ALU_SET_LESS, NULL);
    return 0;
}



int j(int address) {
    PC = ((PC + 4) & 0xf0000000) | (address << 2);
    return 0;
}

int jal(int address) {
    R[$ra] = PC;
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
    R[rt] = ALU(R[rs], imm, ALU_ADD, NULL);
    return 0;
}

int slti(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, ALU_SET_LESS, NULL);
    return 0;
}

int andi(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, ALU_AND, NULL);
    return 0;
}


int ori(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, ALU_OR, NULL);
    return 0;
}

int xori(int rt, int rs, int imm) {
    R[rt] = ALU(R[rs], imm, ALU_XOR, NULL);
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

    if (opcode == R_FORMAT) {
        switch (funct) {
        case SLL:     return sll(rd, rt, sh);
        case SRL:     return srl(rd, rt, sh);
        case SRA:     return sra(rd, rt, sh);
        case JR:      return jr(rs);
        case SYSCALL: return syscall();
        case MFHI:    return mfhi(rd);
        case MFLO:    return mflo(rd);
        case MUL:     return mul(rs, rt);
        case ADD:     return add(rd, rs, rt);
        case SUB:     return sub(rd, rs, rt);
        case AND:     return and (rd, rs, rt);
        case OR:      return or (rd, rs, rt);
        case XOR:     return xor (rd, rs, rt);
        case NOR:     return nor(rd, rs, rt);
        case SLT:     return slt(rd, rs, rt);
        default:      printf("잘못된 명령어 입니다. \n");
        }
    }
    else {
        switch (opcode) {
        case J:    return j(address);
        case JAL:  return jal(address);
        case BLTZ: return bltz(rs, rt, imm);
        case BEQ:  return beq(rs, rt, imm);
        case BNE:  return bne(rs, rt, imm);
        case ADDI: return addi(rt, rs, imm);
        case SLTI: return slti(rt, rs, imm);
        case ANDI: return andi(rt, rs, u_imm);
        case ORI:  return ori(rt, rs, u_imm);
        case XORI: return xori(rt, rs, u_imm);
        case LUI:  return lui(rt, u_imm);
        case LW:   return lw(rt, imm, rs);
        case SW:   return sw(rt, imm, rs);
        case LB:   return lb(rt, imm, rs);
        case SB:   return sb(rt, imm, rs);
        case LBU:  return lbu(rt, imm, rs);
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

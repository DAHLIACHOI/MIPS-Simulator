#define REGISTER_SIZE 32
#define MEMORY_SIZE 0x100000

union instructionRegister {
    unsigned int i; 

    struct RType {
        unsigned int fct : 6;
        unsigned int sht : 5;
        unsigned int rd : 5;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opc : 6;
    } R;

    struct IType {
        int offset : 16;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opc : 6;
    } I;

    struct JType {
        unsigned int jval : 26;
        unsigned int opc : 6;
    } J;
};

// OPCODE
enum OPCODE {
    R_FORMAT = 0,
    BLTZ = 1,
    J = 2,
    JAL = 3,
    BEQ = 4,
    BNE = 5,
    ADDI = 8,
    ADDIU = 9,
    SLTI = 10,
    SLTIU = 11,
    ANDI = 12,
    ORI = 13,
    XORI = 14,
    LUI = 15,
    LB = 32,
    LW = 35,
    LBU = 36,
    SB = 40,
    SW = 43
};

// FUNCT
enum FUNCT {
    SLL = 0,
    SRL = 2,
    SRA = 3,
    JR = 8,
    SYSCALL = 12,
    MFHI = 16,
    MFLO = 18,
    MUL = 24,
    DIV = 26,
    ADD = 32,
    SUB = 34,
    AND = 36,
    OR,
    XOR,
    NOR,
    SLT = 42
};


static char* OPCODE[6][8] =
{
	{"R","bltz","j","jal","beq","bne","NULL","NULL"},
	{"addi","NULL","slti","NULL","andi","ori","xori","lui"},
	{"NULL","NULL","NULL","NULL","NULL","NULL","NULL","NULL"},
	{"NULL","NULL","NULL","NULL","NULL","NULL","NULL","NULL"},
	{"lb","NULL","NULL","lw","lbu","NULL","NULL","NULL"},
	{"sb","NULL","NULL","sw","NULL","NULL","NULL","NULL"}

};

static char* FUNCT[6][8] =
{
	{"sll","NULL", "srl","sra","NULL","NULL","NULL","NULL"},
	{"jr", "NULL","NULL","NULL","syscall","NULL","NULL","NULL"},
	{"mfhi","NULL""mflo","NULL","NULL","NULL","NULL","NULL"},
	{"mul","NULL","NULL","NULL","NULL","NULL","NULL","NULL"},
	{"add","NULL","sub","NULL","and","or","xor","nor"},
	{"NULL","NULL","slt","NULL","NULL","NULL","NULL","NULL"}
};


enum acessSize { BYTE, HALF, WORD };

enum accessType { RD, WR };

enum REGISTER_ENUM {
    $zero, $at, $v0, $v1, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $t3, $t4, $t5, $t6, $g7, $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $t8, $t9, $k0, $k1, $gp, $sp, $fp, $ra
};

static char REGISTER_STR[REGISTER_SIZE][6] = {
  "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$g7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};



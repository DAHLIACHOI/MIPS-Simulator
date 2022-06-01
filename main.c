#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

#define MAX 3 // �ִ� �Է� ���� 3������ ex) sr <register number> <value>
#define M_SIZE 1024

unsigned int parseInt(char* str);
int readChar(FILE* fp, unsigned char* c);
int readCommands(char** commands);
void loadProgram(const char* filename);
extern int PC;

int main(void) {
    while (1) {
        char* commands[MAX] = { "", "", "" };
        int argc = readCommands(commands);

        switch (commands[0][0]) 
        {
        case 'h':
            printf("Load program : l <���� �����̸�>\n");
            printf("Jump program : j <���α׷� ���� ��ġ>\n");
            printf("Go program : g\n");
            printf("Step : s\n");
            printf("View memory : m <start> <end>\n");
            printf("Program exit : x\n");
            printf("Ư�� �������� �� ���� : sr <register number> <value>\n");
            printf("�޸� Ư�� �ּ��� �� ���� : sm <location> <value>\n");
            printf("\n");
            break;
            
        case 'l': 
            char* filename = commands[1];
            loadProgram(filename);
            break;
        case 'j':
            unsigned int address = parseInt(commands[1]);
            jumpProgram(address);
            break;
        case 'g':
            goProgram();
            break;
        case 's':
            switch (commands[0][1]) {
            case 0: 
                stepProgram();
                break;
            case 'm':
                unsigned int address = parseInt(commands[1]);
                int memory_value = parseInt(commands[2]);
                setMemory(address, memory_value);
                break;
            case 'r': 
                unsigned int number = parseInt(commands[1]);
                int register_value = parseInt(commands[2]);
                setRegister(number, register_value);
                break;
            }
            break;
        case 'm': 
            unsigned int start = parseInt(commands[1]);
            unsigned int end = parseInt(commands[2]);
            viewMemory(start, end);
            break;
        case 'r': 
            viewRegister();
            break;
        case 'x':
            printf("���α׷��� ����Ǿ����ϴ�.\n");
            exit(1);

        default:
            printf("�߸��� ��ɾ� �Դϴ�. ������ ���Ͻø� h�� �Է��ϼ���...\n");
        }
        for (int i = 0; i < argc; i++) free(commands[i]); // �����Ҵ��Ѱ� �������ֱ�
    }

    return 0;
}


int readCommands(char** commands) {

    printf("��ɾ �Է����ּ���: ");
    char commandBuffer[100];
    fgets(commandBuffer, 100, stdin);

    strtok(commandBuffer, "\n");
    char* token = strtok(commandBuffer, " ");
    int argc = 0; 
    while (token != NULL && argc < MAX) { 
        commands[argc] = (char*)malloc((strlen(token) + 1) * sizeof(char)); 
        strcpy(commands[argc], token); 
        argc++; 
        token = strtok(NULL, " "); 
    }

    return argc;
}

unsigned int parseInt(char* str) {
    return (unsigned int)strtoul(str, NULL, 0);
}

int readChar(FILE* fp, unsigned char* c) {
    if (fread(c, 1, 1, fp) != 1) return 1;
    else return 0;
}

unsigned int invertEndian(unsigned int data)
{
    unsigned char c[4];

    c[3] = (unsigned char)data; data = data >> 8;
    c[2] = (unsigned char)data; data = data >> 8;
    c[1] = (unsigned char)data; data = data >> 8;
    c[0] = (unsigned char)data;

    return *(unsigned int*)c;
}

void loadProgram(const char* filename) {
    FILE* pFile = NULL;
    errno_t err;
    unsigned int data;
    unsigned int addr;
    unsigned int iCount;
    unsigned int dCount;

    err = fopen_s(&pFile, filename, "rb");
    if (err) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }

    unsigned char BUFFER[M_SIZE];

    for (int i = 0; i < M_SIZE; i++) {
        int err = readChar(pFile, &BUFFER[i]);
        if (err) break;
    }

    fclose(pFile); 

    iCount = invertEndian(((unsigned int*)BUFFER)[0]);
    dCount = invertEndian(((unsigned int*)BUFFER)[1]);

    printf("Number of Instructions: %d, Number of Data: %d\n", iCount, dCount);

    PC = 0x400000;
    resetMemory();
    resetRegister();

    for (int i = 0; i < iCount; i++) {
        int offset = i + 2;
        unsigned int word = invertEndian(((unsigned int*)BUFFER)[offset]);
        MEM(0x400000 + 4 * i, word, WRITE, WORD);
    }

    for (int i = 0; i < dCount; i++) {
        int offset = i + 2 + iCount; 
        unsigned int word = invertEndian(((unsigned int*)BUFFER)[offset]);
        MEM(0x10000000 + 4 * i, word, WRITE, WORD);
    }

    printf("���������� �ε�Ǿ����ϴ�.\n\n");
}
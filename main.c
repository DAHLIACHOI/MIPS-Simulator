#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

#define MAX 3 // 최대 입력 인자 3개까지 ex) sr <register number> <value>
#define M_SIZE 1024

int readChar(FILE* fp, unsigned char* c);
void loadProgram(const char* filename);
extern int PC;

int main(void) {

    while (1) {
        char commands[3][100];
        char str[100];

        printf("커맨드를 입력해주세요: ");
        gets(str);

        char* ptr = strtok(str, " ");
        int i = 0;

        while (ptr != NULL) {
            strcpy(commands[i++], ptr);
            ptr = strtok(NULL, " ");
        }

        if (strcmp(commands[0], "h") == 0) {

            printf("Load program : l <실행 파일이름>\n");
            printf("Jump program : j <프로그램 시작 위치>\n");
            printf("Go program : g\n");
            printf("Step : s\n");
            printf("View memory : m <start> <end>\n");
            printf("Program exit : x\n");
            printf("특정 레지스터 값 설정 : sr <register number> <value>\n");
            printf("메모리 특정 주소의 값 설정 : sm <location> <value>\n");
            printf("현재 PC값 확인 : pc\n");
            printf("\n");

        }

        if (strcmp(commands[0], "l") == 0) {
            char* filename = commands[1];
            loadProgram(filename);
        }

        else if (strcmp(commands[0], "j") == 0) {
            unsigned int address = (unsigned int)strtoul(commands[1], NULL, 0);
            jumpProgram(address);
        }

        else if (strcmp(commands[0], "g") == 0) {
            goProgram();
        }

        else if (strcmp(commands[0], "s") == 0) {
            stepProgram();
        }

        else if (strcmp(commands[0], "sm") == 0) {
            unsigned int address = (unsigned int)strtoul(commands[1], NULL, 0);
            int memory_value = (unsigned int)strtoul(commands[2], NULL, 0);
            setMemory(address, memory_value);
        }

        else if (strcmp(commands[0], "sr") == 0) {
            unsigned int number = (unsigned int)strtoul(commands[1], NULL, 0);
            int register_value = (unsigned int)strtoul(commands[2], NULL, 0);
            setRegister(number, register_value);
        }

        else if (strcmp(commands[0], "m") == 0) {
            unsigned int start = (unsigned int)strtoul(commands[1], NULL, 0);
            unsigned int end = (unsigned int)strtoul(commands[2], NULL, 0);
            viewMemory(start, end);
        }

        else if (strcmp(commands[0], "r") == 0) {
            viewRegister();
        }

        else if (strcmp(commands[0], "pc") == 0) {
            printf("현재 PC 값은 : [%x] 입니다.\n\n", PC);
        }

        else if (strcmp(commands[0], "x") == 0) {
            printf("프로그램이 종료되었습니다.\n");
            exit(0);
        }

        else {
            printf("잘못된 명령어 입니다. 도움을 원하시면 h를 입력하세요...\n\n");
        }
    }

    return 0;
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
        printf("파일을 열람할 수 없습니다 : %s\n", filename);
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
        MEM(0x400000 + 4 * i, word, WR, WORD);
    }

    for (int i = 0; i < dCount; i++) {
        int offset = i + 2 + iCount; 
        unsigned int word = invertEndian(((unsigned int*)BUFFER)[offset]);
        MEM(0x10000000 + 4 * i, word, WR, WORD);
    }

    printf("성공적으로 로드되었습니다.\n\n");
}

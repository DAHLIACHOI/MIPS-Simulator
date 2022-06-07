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
            int value = (unsigned int)strtoul(commands[2], NULL, 0);
            setMemory(address, value);
        }

        else if (strcmp(commands[0], "sr") == 0) {
            unsigned int number = (unsigned int)strtoul(commands[1], NULL, 0);
            int value = (unsigned int)strtoul(commands[2], NULL, 0);
            setRegister(number, value);
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 10
#define MAX_CHARS 100

char savedText[MAX_LINES][MAX_CHARS];
int totalLines = 0;

void printLines(char *input) {
    if (strcmp(input, "*") == 0) {
        for (int i = 0; i < totalLines; i++) {
            printf("%s\n", savedText[i]);
        }
    }
    else if (strchr(input, '-') != NULL) {
        int start, end;
        sscanf(input, "%d-%d", &start, &end);
        if (start < 1) start = 1;
        if (end > totalLines) end = totalLines;
        for (int i = start - 1; i < end; i++) {
            printf("%s\n", savedText[i]);
        }
    }
    else if (strchr(input, ',') != NULL) {
        char *token = strtok(input, ",");
        while (token != NULL) {
            int lineNum = atoi(token);
            if (lineNum >= 1 && lineNum <= totalLines) {
                printf("%s\n", savedText[lineNum - 1]);
            }
            token = strtok(NULL, ",");
        }
    }
    else {
        int lineNum = atoi(input);
        if (lineNum >= 1 && lineNum <= totalLines) {
            printf("%s\n", savedText[lineNum - 1]);
        } else {
            printf("Error: 입력한 줄 번호가 범위를 벗어났습니다.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }
    while (fgets(savedText[totalLines], MAX_CHARS, fp) != NULL) {
        size_t len = strlen(savedText[totalLines]);
        if (len > 0 && savedText[totalLines][len - 1] == '\n') {
            savedText[totalLines][len - 1] = '\0';
        }
        totalLines++;
        if (totalLines >= MAX_LINES) break;
    }
    fclose(fp);

    printf("File read success\n");
    printf("Total Line : %d\n", totalLines);
    printf("You can choose 1 ~ %d Line\n", totalLines);
    printf("Pls 'Enter' the line to select : ");

    char input[100];
    fgets(input, sizeof(input), stdin);
    size_t inputLen = strlen(input);
    if (inputLen > 0 && input[inputLen - 1] == '\n') {
        input[inputLen - 1] = '\0';
    }

    printLines(input);

    return 0;
}

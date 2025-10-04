#include <stdio.h>
#include <string.h>

#define MAX_LINES 10
#define MAX_CHARS 100

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    char savedText[MAX_LINES][MAX_CHARS];
    int totalLines = 0;

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
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

    for (int i = totalLines - 1; i >= 0; i--) {
        printf("%s\n", savedText[i]);
    }

    return 0;
}

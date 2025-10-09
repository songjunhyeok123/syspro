#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_file_content(FILE *fp, int show_line_numbers, long long *current_line_number) {
    int c;
    int at_start_of_line = 1;

    while ((c = fgetc(fp)) != EOF) {
        if (show_line_numbers && at_start_of_line) {
            printf("%6lld\t", (*current_line_number)++);
            at_start_of_line = 0;
        }
        putchar(c);
        if (c == '\n') {
            at_start_of_line = 1;
        }
    }
}

int main(int argc, char *argv[]) {
    int show_line_numbers = 0;
    int start_file_index = 1;

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        show_line_numbers = 1;
        start_file_index = 2;
    }

    if (argc == start_file_index) {
        long long line_number_counter = 1;
        display_file_content(stdin, show_line_numbers, &line_number_counter);
    } else {
        long long line_number_counter = 1;

        for (int i = start_file_index; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");

            if (fp == NULL) {
                fprintf(stderr, "cat: '%s': No such file or directory\n", argv[i]);
                continue;
            }

            display_file_content(fp, show_line_numbers, &line_number_counter);
            fclose(fp);
        }
    }

    return 0;
}

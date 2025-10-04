#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "student.h"

#define RECORD_START_POS 1401001

void print_record(student *rec) {
    printf("Name:%-10s StuID:%d Score:%d\n", rec->name, rec->id, rec->score);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "How to use: %s FileName\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("File open error");
        return 2;
    }

    char ans;
    int input_id;
    student rec;
    ssize_t read_bytes;
    int found;

    do {
        printf("Enter StudentID to search:");
        if (scanf("%d", &input_id) != 1) {
            fprintf(stderr, "fail\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        found = 0;
        off_t current_file_pos = RECORD_START_POS;
        while (1) {
            if (lseek(fd, current_file_pos, SEEK_SET) == (off_t)-1) {
                perror("lseek error");
                close(fd);
                return 3;
            }

            read_bytes = read(fd, &rec, sizeof(student));

            if (read_bytes == 0) break;
			else if (read_bytes < (ssize_t)sizeof(student)) {
				fprintf(stderr, "WARNNING\n");
				break;
			}
            else if (read_bytes < 0) {
                perror("read error");
                close(fd);
                return 4;
            }

            if (rec.id == 0) {
                current_file_pos += sizeof(student);
                continue;
            }

            if (rec.id == input_id) {
                print_record(&rec);
                found = 1;
                break;
            }
            current_file_pos += sizeof(student);
        }

        if (!found) {
            printf("Record %d Null\n", input_id);
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        printf("Continue?(Y/N):");
        ans = getchar();
        while ((c = getchar()) != '\n' && c != EOF) {}

        ans = toupper(ans);

    } while(ans == 'Y');

    close(fd);
    return 0;
}

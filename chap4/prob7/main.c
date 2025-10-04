#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "student.h"

#define RECORD_START_POS 1401001

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <record_file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("Error opening file");
        return 2;
    }

    char ans;
    int input_id;
    student current_rec;
    ssize_t read_bytes;
    int found;

    do {
        printf("Enter StudentID to be modified: ");
        if (scanf("%d", &input_id) != 1) {
            fprintf(stderr, "Input error\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        found = 0;
        off_t current_file_pos = RECORD_START_POS;

        while (1) {
            if (lseek(fd, current_file_pos, SEEK_SET) == (off_t)-1) {
                break;
            }

            read_bytes = read(fd, &current_rec, sizeof(student));

            if (read_bytes == 0) {
                break;
            } else if (read_bytes < (ssize_t)sizeof(student)) {
                fprintf(stderr, "Warning: incomplete record found\n");
                break;
            } else if (read_bytes < 0) {
                perror("Read error");
                close(fd);
                return 3;
            }

            if (current_rec.id == 0) {
                current_file_pos += sizeof(student);
                continue;
            }

            if (current_rec.id == input_id) {
                printf("StuID:%d   Name:%s   Score:%d\n", current_rec.id, current_rec.name, current_rec.score);
                printf("New Score: ");
                int new_score;
                if (scanf("%d", &new_score) != 1) {
                    fprintf(stderr, "Input error\n");
                    clear_input_buffer();
                    found = 1;
                    break;
                }
                clear_input_buffer();

                current_rec.score = new_score;

                if (lseek(fd, current_file_pos, SEEK_SET) == (off_t)-1) {
                    perror("Error seeking to record position");
                    close(fd);
                    return 4;
                }

                if (write(fd, &current_rec, sizeof(student)) != sizeof(student)) {
                    perror("Error writing record");
                    close(fd);
                    return 5;
                }
                printf("Record updated: StudentID %d score set to %d\n", input_id, new_score);
                found = 1;
                break;
            }
            current_file_pos += sizeof(student);
        }

        if (!found) {
            printf("Record %d not found\n", input_id);
        }

        printf("Continue? (Y/N): ");
        ans = getchar();
        clear_input_buffer();
        ans = toupper(ans);

    } while(ans == 'Y');

    close(fd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "student.h"

#define RECORD_START_POS 1401001

int main(int argc, char *argv[]) {
	printf("StuID     Name     Score\n");
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <record_file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT, 0640);
    if (fd < 0) {
        perror("open() error");
        return 1;
    }

    student s;
    int count = 0;
    off_t offset;

    while (scanf("%d %23s %hd", &s.id, s.name, &s.score) == 3) {
        offset = RECORD_START_POS + count * sizeof(student);
        if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
            perror("lseek() error");
            close(fd);
            return 1;
        }

        ssize_t written = write(fd, &s, sizeof(student));
        if (written != sizeof(student)) {
            perror("write() error");
            close(fd);
            return 1;
        }

        count++;
    }

    if (count < 2) {
        close(fd);
        return 1;
    }

    close(fd);

    return 0;
}

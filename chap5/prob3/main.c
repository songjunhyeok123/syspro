#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <dest_file>\n", argv[0]);
        return 1;
    }

 
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        return 1;
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (dest_fd < 0) {
        perror("Error creating destination file");
        close(src_fd);
        return 1;
    }

    char buffer[BUFSIZE];
    ssize_t bytes_read;

   
    while ((bytes_read = read(src_fd, buffer, BUFSIZE)) > 0) {
        ssize_t bytes_written = 0;
        ssize_t ret;
      
        while (bytes_written < bytes_read) {
            ret = write(dest_fd, buffer + bytes_written, bytes_read - bytes_written);
            if (ret < 0) {
                perror("Error writing to destination file");
                close(src_fd);
                close(dest_fd);
                return 1;
            }
            bytes_written += ret;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading source file");
    }

    if (fflush(NULL) == EOF) {
        perror("Error flushing output buffer");
    }

    close(src_fd);
    close(dest_fd);

    return 0;
}

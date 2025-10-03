#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int fd, fd_dup;
	ssize_t write_len;
	const char *file_name = "myfile";

	fd = creat(file_name, 0600);
	if (fd<0){
		perror("creat() error");
		exit(1);
	}

	write_len = write(fd, "Hello! Linux", 12);
	if (write_len != 12){
		perror("write() error");
		close(fd);
		exit(1);
	}

	fd_dup = dup(fd);
	if (fd_dup < 0) {
		perror("dup() error");
		close(fd);
		exit(1);
	}

	write_len = write(fd_dup, "Bye! Linux", 10);
	if (write_len != 10){
		perror("write() error");
		close(fd);
		close(fd_dup);
		exit(1);
	}
	close(fd);
	close(fd_dup);

	return 0;
}

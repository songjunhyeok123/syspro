// main.c  (gcc -Wall -Wextra -o myshell main.c 로 컴파일해봐)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 128

// 앞뒤 공백 제거 함수
char *trim(char *s) {
    char *end;

    // 앞쪽 공백 제거
    while (*s == ' ' || *s == '\t' || *s == '\n')
        s++;

    if (*s == 0) return s;   // 전부 공백이었던 경우

    // 뒤쪽 공백 제거
    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\n'))
        *end-- = '\0';

    return s;
}

// 한 개의 명령 실행 함수
void run_command(char *cmd) {
    char *argv[MAX_ARGS];
    int argc = 0;
    int background = 0;
    char *infile = NULL;
    char *outfile = NULL;

    cmd = trim(cmd);
    if (strlen(cmd) == 0) return;

    // 맨 끝에 & 가 있으면 백그라운드 실행
    int len = strlen(cmd);
    while (len > 0 && (cmd[len-1] == ' ' || cmd[len-1] == '\t'))
        cmd[--len] = '\0';
    if (len > 0 && cmd[len-1] == '&') {
        background = 1;
        cmd[len-1] = '\0';
        cmd = trim(cmd);
    }

    // 공백 기준으로 토큰 자르기
    char *token = strtok(cmd, " \t");
    while (token != NULL && argc < MAX_ARGS-1) {
        if (strcmp(token, "<") == 0) {          // 입력 리다이렉션
            token = strtok(NULL, " \t");
            if (token) infile = token;
        } else if (strcmp(token, ">") == 0) {   // 출력 리다이렉션
            token = strtok(NULL, " \t");
            if (token) outfile = token;
        } else {
            argv[argc++] = token;
        }
        token = strtok(NULL, " \t");
    }
    argv[argc] = NULL;

    if (argc == 0) return;

    // 내장 명령: exit
    if (strcmp(argv[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // 자식 프로세스: 리다이렉션 설정
        if (infile) {
            int fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror("open infile");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (outfile) {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open outfile");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(argv[0], argv);
        perror("execvp");   // 명령 실행 실패
        exit(1);
    } else {
        // 부모 프로세스
        if (!background) {
            int status;
            waitpid(pid, &status, 0);   // foreground: 기다림
        } else {
            printf("[background pid %d]\n", pid);
        }
    }
}

int main(void) {
    char line[MAX_LINE];

    while (1) {
        printf("[shell] ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        // ; 로 구분된 여러 명령 처리
        char *cmd = strtok(line, ";");
        while (cmd != NULL) {
            run_command(cmd);
            cmd = strtok(NULL, ";");
        }
    }
    return 0;
}


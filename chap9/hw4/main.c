#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h> // 에러 처리를 위해 추가

#define MAXLINE 1024
#define MAXARG  64

int main(void)
{
    char line[MAXLINE];
    char *args[MAXARG]; // **수정: char * 타입으로 변경**
    int status; // waitpid 사용을 위해 추가

    while (1) {
        // **개선: 좀비 프로세스 수거 (non-blocking wait)**
        while (waitpid(-1, &status, WNOHANG) > 0);
        
        printf("[Shell] "); // 1) 프롬프트 출력
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break; // EOF (Ctrl+D 등) 이면 종료
        }

        // 개행 문자 제거
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n')
            line[len-1] = '\0';

        // 공백만 있는 줄이면 다시 입력
        char *p = line; // **수정: 포인터 선언**
        while (*p == ' ' || *p == '\t') p++; // **수정: *p로 역참조**
        if (*p == '\0')
            continue;

        // 뒤에 '&' 가 있으면 백그라운드 실행
        int background = 0;
        len = strlen(line);
        // 후행 공백/탭 제거
        while (len > 0 && (line[len-1] == ' ' || line[len-1] == '\t'))
            line[--len] = '\0';
            
        if (len > 0 && line[len-1] == '&') {
            background = 1;
            line[len-1] = '\0';
            // & 제거 후 남은 후행 공백/탭 제거 (2차 정리)
            while (len > 0 && (line[len-1] == ' ' || line[len-1] == '\t'))
                line[--len] = '\0';
        }
        
        // 2) strtok_r 로 인자 분리
        int argc = 0;
        char *saveptr; // **수정: char * 타입으로 변경**
        char *token = strtok_r(line, " \t", &saveptr);
        
        while (token != NULL && argc < MAXARG - 1) {
            args[argc++] = token;
            token = strtok_r(NULL, " \t", &saveptr);
        }
        args[argc] = NULL;

        if (argc == 0)      // 아무 인자 없으면 다시
            continue;

        // 내장 명령: exit
        if (strcmp(args[0], "exit") == 0)
            break;

        // 3) 자식 프로세스 생성 후 execvp 로 명령 실행
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // 자식 프로세스
            execvp(args[0], args);
            perror("execvp");    // 실패 시
            exit(1);
        } else {
            // 4) 부모는 자식이 끝나길 기다리거나 (전면 실행),
            //    & 가 있으면 기다리지 않고 다음 프롬프트로.
            if (!background) {
                int status;
                waitpid(pid, &status, 0); // 전면 실행: 기다림
            } else {
                printf("[Shell] background pid %d\n", pid); // 후면 실행: PID 출력 후 계속
            }
        }
        // 5) while 루프로 돌아가서 반복
    }

    return 0;
}

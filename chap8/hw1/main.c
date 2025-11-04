#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>   
#include <string.h>   

extern char **environ;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s [옵션1] [옵션2] ...\n", argv[0]);
        fprintf(stderr, "  -e[환경변수이름] : 지정된 환경변수 값 출력. 이름이 없으면 모든 환경변수 출력.\n");
        fprintf(stderr, "  -u             : 실제/유효 사용자 ID 출력.\n");
        fprintf(stderr, "  -g             : 실제/유효 그룹 ID 출력.\n");
        fprintf(stderr, "  -i             : 프로세스 ID 출력.\n");
        fprintf(stderr, "  -p             : 부모 프로세스 ID 출력.\n");
        return 1; 
    }

    // 주어진 옵션들을 하나씩 살펴볼게요!
    for (int i = 1; i < argc; i++) {
       
        if (strncmp(argv[i], "-e", 2) == 0) {
            if (strlen(argv[i]) > 2) {
                char *env_name = argv[i] + 2; 
                char *env_val = getenv(env_name); 
                if (env_val != NULL) {
                    printf("환경 변수 '%s' 값: %s\n", env_name, env_val);
                } else {
                    printf("환경 변수 '%s'를 찾을 수 없습니다.\n", env_name);
                }
            } else { 
                printf("--- 모든 환경 변수 ---\n");
                char **env = environ; 
                while (*env) { 
                    printf("  %s\n", *env);
                    env++;
                }
                printf("----------------------\n");
            }
        }
        else if (strcmp(argv[i], "-u") == 0) {
            printf("실제 사용자 ID (RUID): %d\n", getuid());
            printf("유효 사용자 ID (EUID): %d\n", geteuid());
        }
        
        else if (strcmp(argv[i], "-g") == 0) {
            printf("실제 그룹 ID (RGID): %d\n", getgid());
            printf("유효 그룹 ID (EGID): %d\n", getegid());
        }
        else if (strcmp(argv[i], "-i") == 0) {
            printf("프로세스 ID (PID): %d\n", getpid());
        }
        else if (strcmp(argv[i], "-p") == 0) {
            printf("부모 프로세스 ID (PPID): %d\n", getppid());
        }
        else {
            fprintf(stderr, "알 수 없는 옵션 또는 잘못된 형식: %s\n", argv[i]);
            fprintf(stderr, "사용법: %s [-e [환경변수이름]] [-u] [-g] [-i] [-p]\n", argv[0]);
            return 1; 
        }
    }

    return 0; 
}

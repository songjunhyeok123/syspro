#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h> // for getopt (though we'll parse manually here)

// 전역 옵션 변수
int opt_i = 0; // inode 번호 출력
int opt_p = 0; // 디렉토리에 '/' 추가
int opt_Q = 0; // 파일 이름을 큰따옴표로 묶음

char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);
void process_dir(char *dir);

int main(int argc, char **argv)
{
    char *dir = ".";

    // 1. 옵션 파싱 (간단한 수동 파싱)
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // 옵션 문자열을 순회
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'i':
                        opt_i = 1;
                        break;
                    case 'p':
                        opt_p = 1;
                        break;
                    case 'Q':
                        opt_Q = 1;
                        break;
                    default:
                        fprintf(stderr, "list2: invalid option -- '%c'\n", argv[i][j]);
                        exit(1);
                }
            }
        } else {
            // 옵션이 아닌 경우 디렉토리로 간주 (여기서는 첫 번째 비-옵션 인자만 디렉토리로 처리)
            dir = argv[i];
            break; // 단순화를 위해 첫 번째 비-옵션 인자만 디렉토리로 처리
        }
    }

    // 2. 디렉토리 처리 실행
    process_dir(dir);

    exit(0);
}

// 디렉토리 목록을 읽고 통계 정보를 출력하는 함수
void process_dir(char *dir)
{
    DIR *dp;
    struct stat st;
    struct dirent *d;
    char path[BUFSIZ+1];

    if ((dp = opendir(dir)) == NULL) {
        perror(dir);
        return; // 실패 시 종료
    }

    while ((d = readdir(dp)) != NULL) {
        // 경로 생성
        sprintf(path, "%s/%s", dir, d->d_name);

        // 파일/디렉토리 정보 가져오기 (심볼릭 링크는 따라가지 않고 자체 정보 가져오기 위해 lstat 사용)
        if (lstat(path, &st) < 0) {
            perror(path);
        } else {
            printStat(path, d->d_name, &st);
        }
    }
    closedir(dp);
}

// ---

// 파일 통계 정보를 출력하는 함수
void printStat(char *pathname, char *file, struct stat *st)
{
    // -i 옵션 처리: inode 번호 출력
    if (opt_i) {
        printf("%8lu ", st->st_ino);
    }
    
    printf("%5ld ", st->st_blocks);
    printf("%c%s ", type(st->st_mode), perm(st->st_mode));
    printf("%3ld ", st->st_nlink);
    printf("%s %s ", getpwuid(st->st_uid)->pw_name,
                 getgrgid(st->st_gid)->gr_name);
    printf("%9ld ", st->st_size);
    printf("%.12s ", ctime(&st->st_mtime)+4);

    // -Q 옵션 처리: 파일 이름에 큰따옴표 추가
    if (opt_Q) {
        printf("\"");
    }

    // 파일 이름 출력
    printf("%s", file);

    // -p 옵션 처리: 디렉토리인 경우 '/' 추가
    if (opt_p && S_ISDIR(st->st_mode)) {
        printf("/");
    }

    // -Q 옵션 처리: 파일 이름에 큰따옴표 닫기
    if (opt_Q) {
        printf("\"");
    }

    printf("\n");
}

// 파일 유형을 반환하는 함수 (기존 코드와 동일)
char type(mode_t mode)
{
    if (S_ISREG(mode))
        return('-');
    if (S_ISDIR(mode))
        return('d');
    if (S_ISCHR(mode))
        return('c');
    if (S_ISBLK(mode))
        return('b');
    if (S_ISLNK(mode))
        return('l');
    if (S_ISFIFO(mode))
        return('p');
    if (S_ISSOCK(mode))
       return('s');
    return '?'; // 안전을 위해 추가
}

// 권한 문자열을 반환하는 함수 (기존 코드와 동일)
char* perm(mode_t mode)
{
    static char perms[10];
    strcpy(perms, "---------");

    // S_IRUSR, S_IWUSR, S_IXUSR 등은 i=0일 때 사용자 권한의 비트입니다.
    // i*3만큼 오른쪽으로 시프트하면 그룹, 기타 권한을 확인할 수 있습니다.
    for (int i=0; i < 3; i++) {
        if (mode & (S_IRUSR >> i*3))
            perms[i*3] = 'r';
        if (mode & (S_IWUSR >> i*3))
            perms[i*3+1] = 'w';
        if (mode & (S_IXUSR >> i*3))
           perms[i*3+2] = 'x';
    }
    return(perms);
}

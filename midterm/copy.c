#include <stdio.h>   // fprintf, perror, printf, putchar, BUFSIZ (버퍼 크기 상수)
#include <stdlib.h>  // atoi, exit, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>  // open, read, write, close 함수
#include <fcntl.h>   // open 함수의 플래그(O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC 등)
#include <ctype.h>   // tolower, toupper 함수 (문자 변환)

int main(int argc, char *argv[]) {
    // 1. 명령줄 인자 개수 확인
    // 프로그램 이름, 옵션, 원본 파일명, 대상 파일명까지 총 4개가 필요해.
    if (argc != 4) {
        fprintf(stderr, "사용법: %s <옵션> <원본 파일명> <대상 파일명>\n", argv[0]);
        fprintf(stderr, "  옵션 0: 원본과 동일하게 복사\n");
        fprintf(stderr, "  옵션 1: 모든 대문자를 소문자로 변환\n");
        fprintf(stderr, "  옵션 2: 모든 소문자를 대문자로 변환\n");
        exit(EXIT_FAILURE); // 프로그램 비정상 종료
    }

    // 2. 옵션 및 파일명 파싱
    int option = atoi(argv[1]); // 문자열 옵션을 정수로 변환
    char *source_filename = argv[2];
    char *destination_filename = argv[3];

    // 3. 옵션 값 유효성 검사
    if (option < 0 || option > 2) {
        fprintf(stderr, "오류: 유효하지 않은 옵션입니다. 옵션은 0, 1, 2 중 하나여야 합니다.\n");
        exit(EXIT_FAILURE);
    }

    // 4. 파일 디스크립터 선언
    int source_fd, dest_fd; // 파일 디스크립터는 정수형이야.

    // 5. 파일 열기 (원본 파일)
    // 원본 파일을 읽기 전용(O_RDONLY)으로 열어.
    source_fd = open(source_filename, O_RDONLY);
    if (source_fd == -1) { // open 실패 시 -1 반환
        perror(source_filename); // 시스템 오류 메시지 출력 (예: No such file or directory)
        exit(EXIT_FAILURE);
    }

    // 6. 파일 열기 (대상 파일)
    // 대상 파일을 쓰기 전용(O_WRONLY)으로 열고, 없으면 생성(O_CREAT)하고,
    // 이미 있으면 내용을 잘라내(O_TRUNC) 비워줘.
    // 0644는 파일 권한 설정이야. (rw-r--r--, 소유자는 읽기/쓰기, 그룹/다른 사용자는 읽기만)
    dest_fd = open(destination_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) { // open 실패 시
        perror(destination_filename);
        close(source_fd); // 원본 파일 디스크립터도 꼭 닫아줘야 해!
        exit(EXIT_FAILURE);
    }

    // 7. 파일 내용 복사 및 수정
    char buffer[BUFSIZ]; // BUFSIZ는 stdio.h에 정의된 표준 버퍼 크기 (보통 8192바이트)
    ssize_t bytes_read;  // read 함수가 반환하는 바이트 수를 저장할 변수

    // 원본 파일에서 BUFSIZ만큼 읽어와. 더 이상 읽을 내용이 없으면 0을 반환, 에러면 -1 반환.
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        // 읽어온 바이트 수만큼 반복하면서 각 문자를 처리해.
        for (int i = 0; i < bytes_read; i++) {
            switch (option) {
                case 1: // 대문자를 소문자로 변환
                    buffer[i] = tolower(buffer[i]);
                    break;
                case 2: // 소문자를 대문자로 변환
                    buffer[i] = toupper(buffer[i]);
                    break;
                case 0: // 원본 그대로 (아무것도 안 함)
                default:
                    break;
            }
        }
        // 수정된(또는 그대로인) 버퍼 내용을 대상 파일에 써줘.
        // write 함수의 반환값이 bytes_read와 다르면 쓰기 실패한 거야.
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("오류: 대상 파일에 쓸 수 없습니다");
            close(source_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    // read 함수가 0보다 작으면 오류 발생!
    if (bytes_read == -1) {
        perror("오류: 원본 파일을 읽을 수 없습니다");
        close(source_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    // 8. 파일 디스크립터 닫기
    close(source_fd);
    close(dest_fd);

    // 9. 복사된 파일(destination.txt)의 내용을 화면에 출력
    printf("\n--- %s 파일 내용 시작 ---\n", destination_filename);
    
    // 복사된 파일을 다시 읽기 모드로 열어서 화면에 출력해야 해.
    // 기존에 dest_fd는 닫았으니 새로 열어야겠지?
    int read_dest_fd = open(destination_filename, O_RDONLY);
    if (read_dest_fd == -1) {
        perror("오류: 복사된 파일을 다시 열 수 없습니다");
        exit(EXIT_FAILURE);
    }

    // 파일 내용을 화면에 출력하기 위해 한 바이트씩 읽어서 `putchar`로 출력
    while ((bytes_read = read(read_dest_fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            putchar(buffer[i]);
        }
    }
    printf("--- %s 파일 내용 끝 ---\n", destination_filename);

    close(read_dest_fd); // 열었던 파일은 꼭 닫아주기!

    return EXIT_SUCCESS; // 프로그램 정상 종료
}

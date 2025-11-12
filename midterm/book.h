#ifndef BOOK_H
#define BOOK_H

#define MAX_TITLE 50
#define MAX_AUTHOR 30
#define DB_FILE "db.dat"

typedef struct {
    int id;               // 책 고유번호 (ID)
    char title[MAX_TITLE]; // 책 이름
    char author[MAX_AUTHOR]; // 저자 이름
    int year;             // 출판년도
    int borrow_count;     // 대출 횟수
    int is_borrowed;      // 대출 유무 (0: 대출 가능(False), 1: 대출 중(True))
} BOOK;

#endif

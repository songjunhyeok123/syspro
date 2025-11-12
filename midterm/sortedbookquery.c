#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 30
#define DB_FILE "db.dat"
#define MAX_BOOKS 100 

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int year;
    int borrow_count;
    int is_borrowed; 
} BOOK;
int compare_by_borrow_count(const void *a, const void *b) {
    const BOOK *bookA = (const BOOK *)a;
    const BOOK *bookB = (const BOOK *)b;
    return bookB->borrow_count - bookA->borrow_count; 
}

void print_header(int type) {
    printf("0: list of all books, 1: list of available books ) %d\n", type);
    printf("id  bookname  author  year  numofborrow  borrow\n");
}

void print_book(const BOOK *book) {
    const char *status = book->is_borrowed ? "True" : "False";
    printf("%-2d  %-8s  %-6s  %-4d  %-11d  %-6s\n",
           book->id, book->title, book->author, book->year, book->borrow_count, status);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    BOOK books[MAX_BOOKS];
    int num_books = 0;
    int query_type;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./sortedbookquery <query_type> (0: All, 1: Available)\n");
        return 1;
    }
    
    query_type = atoi(argv[1]);

    if ((fp = fopen(DB_FILE, "rb")) == NULL) {
        perror("Error opening database file for reading");
        return 1;
    }
    while (fread(&books[num_books], sizeof(BOOK), 1, fp) == 1) {
        num_books++;
        if (num_books >= MAX_BOOKS) break;
    }
    fclose(fp);
    
    qsort(books, num_books, sizeof(BOOK), compare_by_borrow_count);
    
    print_header(query_type);
    
    int displayed_count = 0;
    for (int i = 0; i < num_books; i++) {
       
        if (query_type == 0 || (query_type == 1 && books[i].is_borrowed == 1)) {
            print_book(&books[i]);
            displayed_count++;
        }
    }
    
    return 0;
}

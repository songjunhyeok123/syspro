#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 30
#define DB_FILE "db.dat"

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int year;
    int borrow_count;
    int is_borrowed; 
} BOOK;
void print_header() {
    printf("ID\tbookname\tauthor\tyear\tnumofborrow\tborrow\n");
}

void print_book(const BOOK *book) {
    const char *status = book->is_borrowed ? "True" : "False";
    printf("%d\t%s\t%s\t%d\t%d\t%s\n",
           book->id, book->title, book->author, book->year, book->borrow_count, status);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    BOOK book;
    char search_title[MAX_TITLE];
    int found = 0;
    if (argc != 2) {
        fprintf(stderr, "Usage: ./searchbook <Input name of book>\n");
        return 1;
    }
    strncpy(search_title, argv[1], MAX_TITLE - 1);
    search_title[MAX_TITLE - 1] = '\0';
    
    printf("Input name of book ) %s\n", search_title);

    if ((fp = fopen(DB_FILE, "rb")) == NULL) {
        perror("Error opening database file for reading");
        return 1;
    }
   
    while (fread(&book, sizeof(BOOK), 1, fp) == 1) {
        
        if (strcmp(book.title, search_title) == 0) {
            if (!found) {
                print_header();
            }
            print_book(&book);
            found = 1;
            break; 
        }
    }
    
    if (!found) {
        printf("not found book name: '%s'\n", search_title);
    }
    
    fclose(fp);
    return 0;
}

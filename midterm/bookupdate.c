#include <stdio.h>
#include <stdlib.h>

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

void print_updated_book(const BOOK *book) {
    printf("ID\tbookname\tauthor\tyear\tnumofborrow\tborrow\n");
    printf("%d\t%s\t%s\t%d\t%d\t%d\n",
           book->id, book->title, book->author, book->year, book->borrow_count, book->is_borrowed);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    BOOK book;
    int command, book_id;
    long record_offset;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: ./bookupdate <command> <book_id>\n");
        fprintf(stderr, "Command 0: Borrow, 1: Return\n");
        return 1;
    }
    
    command = atoi(argv[1]);
    book_id = atoi(argv[2]);
    
    if (book_id <= 0) {
        fprintf(stderr, "Invalid Book ID.\n");
        return 1;
    }

    if ((fp = fopen(DB_FILE, "rb+")) == NULL) {
        perror("Error opening database file for updating");
        return 1;
    }
    
    record_offset = (long)(book_id - 1) * sizeof(BOOK);
    
    if (fseek(fp, record_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Book ID %d not found (Seek error or file too short).\n", book_id);
        fclose(fp);
        return 1;
    }
    
    if (fread(&book, sizeof(BOOK), 1, fp) != 1) {
        fprintf(stderr, "Error: Book ID %d not found (Read error).\n", book_id);
        fclose(fp);
        return 1;
    }
    
    if (command == 0) {
        printf("B(bookid): borrow book, R(bookid): return book ) 0 %d\n", book_id);
        if (book.is_borrowed == 1) {
            printf("You cannot borrow book since it has been booked.\n");
            
        } else {
            book.is_borrowed = 1; 
            book.borrow_count++;  
            printf("You've got bellow book...\n");

            fseek(fp, record_offset, SEEK_SET); 
            fwrite(&book, sizeof(BOOK), 1, fp); 
        }
    } 
    else if (command == 1) {
        printf("B(bookid): borrow book, R(bookid): return book ) 1 %d\n", book_id);
        if (book.is_borrowed == 0) {
            printf("You've already returned book.\n");
            
        } else {
            book.is_borrowed = 0; 
            printf("You've returned bellow book...\n");

            fseek(fp, record_offset, SEEK_SET); 
            fwrite(&book, sizeof(BOOK), 1, fp); 
        }
    } 
    else {
        fprintf(stderr, "Invalid command: %d. Use 0 for Borrow, 1 for Return.\n", command);
        fclose(fp);
        return 1;
    }
    if (command == 0 || command == 1) {
        print_updated_book(&book);
    }
    
    fclose(fp);
    return 0;
}

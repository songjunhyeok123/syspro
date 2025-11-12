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

int main() {
    FILE *fp;
    BOOK books[] = {
        {1, "1984", "George", 1949, 7, 0},    
        {2, "Luster", "Raven", 2020, 3, 1},   
        {3, "Hamnet", "Maggie", 2020, 0, 0},  
        {4, "theWish", "Nocholas", 2021, 2, 1}
    };
    int num_books = sizeof(books) / sizeof(BOOK);
    
    
    if ((fp = fopen(DB_FILE, "wb")) == NULL) {
        perror("Error opening database file for writing");
        return 1;
    }
    
    if (fwrite(books, sizeof(BOOK), num_books, fp) != num_books) {
        fprintf(stderr, "Error writing all records to file\n");
        fclose(fp);
        return 1;
    }
    
    fclose(fp);
    printf("✅ Initial book data successfully created and saved to %s\n", DB_FILE);
    
    return 0;
} 

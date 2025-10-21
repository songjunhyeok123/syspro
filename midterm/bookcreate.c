#include <stdio.h>
#include <stdlib.h>
#include "book.h"
int main(int argc, char* argv[])  
{ 
   struct book rec;
   FILE *fp;

   if (argc != 2) {
      fprintf(stderr, "How to use: %s FileName\n",argv[0]);
      exit(1); 
   }

   fp = fopen(argv[1], "wb");
   printf("%-9s %-9s %-9s %-9s %-9s %-9s\n", "책 고유 번호", "책 이름", "저자 이름", "출판 년도", "대출 횟수", "대출 유무"); 
   while (scanf("%d %s %s %d %d %d", &id, bookname, author, &year, &numofborrow, &borrow) == 6) 
      fwrite(&rec, sizeof(rec), 1, fp);

   fclose(fp);
   exit(0);
} 

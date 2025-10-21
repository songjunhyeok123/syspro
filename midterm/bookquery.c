#include <stdio.h> 
#include "book.h"

int main(int argc, char* argv[]) 
{ 
   struct book rec;
   FILE *fp;
   if (argc != 2) { 
      fprintf(stderr, "How to use: %s FileName\n", argv[0]);
      return 1; 
   }
   if ((fp = fopen(argv[1], "rb")) == NULL ) {
      fprintf(stderr, "Error Open File\n");
      return 2;
   }
   printf("-----------------------------------\n");
   printf("%5s %10s %10s %5s %10s %5s\n", "책 고유 번호", "책 이름", "저자 이름", "출판 년도", "대출 횟수", "대출 유무");
   printf("-----------------------------------\n");
   
   while (fread(&rec, sizeof(rec), 1, fp) > 0) 
      if (rec.id != 0)
         printf("%5d %10s %10s %5d %10d %5d\n", &id, bookname, author, &year, &numofborrow, &borrow);
   
   printf("-----------------------------------\n");
   fclose(fp);
   return 0;
}

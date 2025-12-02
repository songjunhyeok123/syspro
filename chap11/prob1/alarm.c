#include <stdio.h>
#include <unistd.h>
/* 알람 시그널을 보여주는 프로그램 */
int main( )
{
   alarm(5);
   printf("Loop start\n");
   short i = 0;
   while (1) {
      sleep(1);
          i++;
      printf("%d second\n", i);
   }
   printf("end\n");
}


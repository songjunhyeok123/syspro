#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void intHandler();
/* 인터럽트 시그널을 처리한다. */
int main( )
{
   signal(SIGINT,intHandler);
   while (1)
      pause();
   printf("end\n");
}
void intHandler(int signo)
{
   printf("Interrupt signal\n"); 
   printf("signal number : %d\n", signo);
   exit(0);
}

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void alarmHandler();
/* 알람 시그널을 처리한다. */
int main( )
{
  signal(SIGALRM, alarmHandler);
  alarm(5);    /* 알람 시간 설정 */
  short i = 0;
  while (1) {
    sleep(1);
    i++;
    printf("%d second\n", i);
  }
  printf("end\n");
}
void alarmHandler(int signo)
{
   printf("Wake up\n");
   exit(0);
}

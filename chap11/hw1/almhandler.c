#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

sighandler_t my_signal(int signum, sighandler_t handler) {
    struct sigaction new_sa;
    struct sigaction old_sa;

    new_sa.sa_handler = handler;
    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = 0;

    if (sigaction(signum, &new_sa, &old_sa) == -1) {
        perror("sigaction");
        return SIG_ERR;
    }
    return old_sa.sa_handler;
}

void alarmHandler(int signo)
{
   printf("Wake up\n");
   exit(0);
}

int main()
{
  my_signal(SIGALRM, alarmHandler);
  alarm(5);
  short i = 0;
  while (1) {
    sleep(1);
    i++;
    printf("%d second\n", i);
  }
  printf("end\n");
}

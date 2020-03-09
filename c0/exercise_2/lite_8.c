#include <signal.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "err.h"


void on_sa(int signal) {
	printf("Otrzymałem sygnał nr: %d\n", signal);
        fflush(stdout);
}

int main ()
{
  struct sigaction setup_action;
  sigset_t block_mask;
 
  struct sigaction sa; //signal action
  sigset_t mask;
  pid_t pid;
  sigemptyset(&mask); /* Wyczyść maskę */
  sa.sa_handler = (&on_sa);
  sa.sa_mask = mask;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGTSTP, &sa, NULL);


  char buf[100];
  int size;
  pid = fork();
  switch(pid) {
    case -1:
      fprintf(stderr, "fork");
    case 0:
      //printf("Jestem dzieckiem o pid=%d\n", getpid());
      sleep(3);
      return getpid() % 10;
    default:
      printf("moj: %d, dziecka: %d\n", getpid(), pid);
      if(kill(pid, SIGTSTP) == -1)
        fprintf(stderr, "KIRR\n");
      return 0;
  }
}

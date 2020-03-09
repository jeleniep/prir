#include <signal.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "err.h"
pid_t parent_pid;

void on_sa(int signal) {
	printf("Otrzymałem sygnał nr: %d\n", signal);
        if(kill(parent_pid, SIGUSR1) == -1)
          fprintf(stderr, "KIRR\n");
}
void on_sa2(int signal) {
        printf("Otrzymałem sygnał pożegnalny od dziecka. Sygnal: %d\n", signal);
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
  sigaction(SIGTERM, &sa, NULL);

  struct sigaction sa2; //signal action
  sigemptyset(&mask); /* Wyczyść maskę */
  sa2.sa_handler = (&on_sa2);
  sa2.sa_mask = mask;
  sa2.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa2, NULL);
  char buf[100];
  int size;
  parent_pid = getpid();
  pid = fork();
  switch(pid) {
    case -1:
      fprintf(stderr, "fork");
    case 0:
      sleep(3);
      return getpid() % 10;
    default:
      printf("moj: %d, dziecka: %d\n", getpid(), pid);
      if(kill(pid, SIGTERM) == -1)
        fprintf(stderr, "KIRR\n");
      sleep(7);
      return 0;
  }
}


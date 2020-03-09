// Napisać program, który tworzy N procesów potomnych.
// Każdy z tych procesów obsługuje sygnał HUP (N potomnych oraz proces pierwotny).
// Po uruchomieniu procesy potomne wykonują czasochłonne zadanie
// (np. poszukują kolejnych liczb pierwszych wypisując je na ekran – wraz ze swoim PID).
// Proces potomny po otrzymaniu sygnału HUP rozpoczyna pracę od nowa.
// Gdy proces nadrzędny otrzyma sygnał HUP, przekazuje go do wszystkich potomków.

#include <signal.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "err.h"
#include <signal.h>

const int N = 10;
int reset = 0;
int children[10];
void on_sa(int signal)
{
  printf("Moj pid: %d Otrzymałem sygnał nr: %d\n", getpid(), signal);
  fflush(stdout);
}

void on_hup_parent(int signal)
{
  int w;
  for (w = 0; w < N; w++)
  {
    kill(children[w], SIGHUP);
  }
}

void on_hup_child(int signal)
{
  reset = 1;
}

void child_work()
{
  int i;
  for (i = 0; i < 30000000; i++)
  {
    if (reset == 1)
    {
      printf("reset\n");
      reset = 2;
      break;
    }
    getpid();
  }
  printf("Jestem dzieckiem o pid=%d\n", getpid());
  if (reset == 2)
  {
    printf("SIEAMJestem dzieckiem o pid=%d %d\n", getpid(), reset);
    reset = 0;
    child_work();
  }
}

int main()
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

  struct sigaction hup;
  sigemptyset(&mask);        /* Wyczyść maskę */
  sigaddset(&mask, SIGTSTP); /* Dodaj do niej SIGUSR1 */
  hup.sa_mask = mask;
  hup.sa_flags = SA_SIGINFO;

  int size;
  int i, j;

  for (j = 0; j < N; j++)
  {
    pid = fork();
    children[j] = pid;
    switch (pid)
    {
    case -1:
      fprintf(stderr, "fork");
    case 0:
      hup.sa_handler = (&on_hup_child);
      sigaction(SIGHUP, &hup, NULL);
      child_work();
      return getpid() % 10;
    default:
      hup.sa_handler = (&on_hup_parent);
      sigaction(SIGHUP, &hup, NULL);
      printf("moj: %d, dziecka: %d\n", getpid(), pid);
    }
  }

  while (1)
    pause();

  // for (j = 0; j < N; j++) {
  //   wait(0);
  // }
}

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> /*deklaracje standardowych funkcji uniksowych (fork(), write() itd.)*/

int main()
{
  pid_t pid;
  printf("Moj PID = %d\n", getpid());
  int i, n = 4, status;
  for (i = 0; i < n; ++i)
  {
    switch (pid = fork())
    {
    case -1:
      fprintf(stderr, "Blad w fork\n");
      status = EXIT_FAILURE;
      break;
    case 0: /*proces potomny*/
      printf("Jestem procesem potomnym. PID = %d\n \ 
                Wartosc przekazana przez fork() = %d\n",
             getpid(), pid);
      return EXIT_SUCCESS;

    default:
      printf("Jestem procesem macierzystym. PID = %d\n \ 
                Wartosc przekazana przez fork() = %d\n",
             getpid(), pid);
      /*Oczekiwanie na zakończenie procesu potomnego*/
      status = EXIT_SUCCESS;
      break;
    }
  }

  for (i = 0; i < n; ++i)
  {
    if (wait(0) == -1)
    {
      fprintf(stderr, "Blad w wait\n");
      status = EXIT_FAILURE;
      break;
    }
  }
  return status;
}

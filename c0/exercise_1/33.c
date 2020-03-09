#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> /*deklaracje standardowych funkcji uniksowych (fork(), write() itd.)*/
#include <time.h>

int main()
{

  pid_t pid;
  printf("Moj PID = %d\n", getpid());

  int i, n = 3, status, isInitialised = 0;
  for (i = 0; i < n; ++i)
  {
    switch (pid = fork())
    {
    case -1:
      fprintf(stderr, "Blad w fork\n");
      status = EXIT_FAILURE;
      break;
    case 0: /*proces potomny*/
            // printf("Jestem procesem potomnym. PID = %d\n \ 
      //           Wartosc przekazana przez fork() = %d\n",
            //        getpid(), pid);
            // sleep(1);
      srand(getpid() * time(NULL));

      printf(" \n WYLOSOWANA LICZBA %d \n", rand() % 6 + 1);

      return EXIT_SUCCESS;

    default:

      if (isInitialised == 0)
      {

        isInitialised = 1;
        printf("po %d %d", i, isInitialised);
      }
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

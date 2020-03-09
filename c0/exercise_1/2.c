#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> /*deklaracje standardowych funkcji uniksowych (fork(), write() itd.)*/

int main()
{
    pid_t pid;
    printf("Moj PID = %d\n", getpid());

    switch(pid = fork()){
        case -1:
            fprintf(stderr, "Blad w fork\n");
            return EXIT_FAILURE;
        case 0: /*proces potomny*/
            printf("Jestem procesem potomnym. PID = %d\n \ 
                Wartosc przekazana przez fork() = %d\n", getpid(), pid);
            return EXIT_SUCCESS;
        default:
            printf("Jestem procesem macierzystym. PID = %d\n \ 
                Wartosc przekazana przez fork() = %d\n", getpid(), pid);
            /*Oczekiwanie na zakończenie procesu potomnego*/
            if(wait(0) == -1)
            {
                fprintf(stderr, "Blad w wait\n");
                return EXIT_FAILURE;

            }
            return EXIT_SUCCESS;

    }

}

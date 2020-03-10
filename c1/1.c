#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "err.h"
#include <signal.h>

#define BUFFOR_SIZE 80
#define BUFSIZE 100

const int N = 2;
int reset = 0;
int *children;

int   shmid;
int* shmaddr;
 
void shm_get()
{
//   char buf[BUFSIZE];
  key_t key;
 
  key = 1;
 
  if( (shmid = shmget(key, N * sizeof(int), 0666 | IPC_CREAT)) < 0)
    printf("shmget error");
  printf("Shared memory ID = %d\n", shmid);
}
 
void shm_attach()
{
  if( (shmaddr = shmat(shmid, 0, 0)) < 0)
    printf("shmat error");
  printf("Shared memory attached\n");
}
 
void shm_detach()
{
  if( shmdt(shmaddr) < 0)
    printf("shmdt error");
  printf("Shared memory detached\n");
}
 
void shm_remove()
{
  if( shmctl(shmid, IPC_RMID, 0) < 0)
    printf("shmctl error");
  printf("Shared memory removed\n");
}
 
// void shm_read()
// {
//   printf("Message: %s\n",shmaddr);
// }
 
// void shm_write()
// {
//   printf("Message: ");
//   fgets(shmaddr, BUFSIZE - 1, stdin);
// }

double sum(double *vector, int n)
{
	int i;
	double sum = 0.0f;
	for (i = 0; i < n; i++)
	{
		sum += vector[i];
	}
	return sum;
}

void on_sa(int signal)
{
	printf("Moj pid: %d Otrzymałem sygnał nr: %d\n", getpid(), signal);
	fflush(stdout);
}

void on_hup_parent(int signal)
{
	int w;
	for (w = 0; w < N; w++)
		;
}

void sig_usr_1(int signal)
{
}

void child_work(int j)
{
  printf("Jestem dzieckiem %d o pid=%d\n", j, getpid());
  shmaddr[j] = 2 * j;
}

int main(int argc, char **argv)
{
	children = malloc(N * sizeof(int));
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

	struct sigaction sigusr;
	sigemptyset(&mask);		   /* Wyczyść maskę */
	sigaddset(&mask, SIGTSTP); /* Dodaj do niej SIGUSR1 */
	sigusr.sa_mask = mask;
	sigusr.sa_flags = SA_SIGINFO;

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
			sigusr.sa_handler = (&sig_usr_1);
			sigaction(SIGUSR1, &sigusr, NULL);
			pause();
			shm_get();
			shm_attach();			
			child_work(j);
			shm_detach();
			return getpid() % 10;
		default:
			printf("moj: %d, dziecka: %d\n", getpid(), pid);
		}
	}
	shm_get();
	shm_attach();
	for (j = 0; j < N; j++)
		wait(NULL);
	for (j = 0; j < N; j++)
		printf("%d ", shmaddr[j]);
	
	shm_detach();
	shm_remove();
	return 0;
}
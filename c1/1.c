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
#include <time.h>

#define BUFFOR_SIZE 80
#define BUFSIZE 100

const int N = 20;
int reset = 0;
int *children;

int data_id;
int *data;

int result_id;
int *results;

int range_id;
int *range;

int shm_get(char *filename, int mode)
{
	//   char buf[BUFSIZE];
	key_t key;
	int id;
	key = ftok(filename, mode);
	printf("%s %d \n", filename, key );
	if ((id = shmget(key, N * sizeof(int), 0666 | IPC_CREAT)) < 0)
		printf("shmget error");
	printf("Shared memory ID = %d\n", id);
	return id;
}

void shm_attach(int **addr, int id)
{
	if ((*addr = shmat(id, 0, 0)) < 0)
		printf("shmat error");
	printf("Shared memory attached\n");
}

void shm_detach(int **addr)
{
	if (shmdt(*addr) < 0)
		printf("shmdt error");
	printf("Shared memory detached\n");
}

void shm_remove(int id)
{
	if (shmctl(id, IPC_RMID, 0) < 0)
		printf("shmctl error");
	printf("Shared memory removed\n");
}


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
	results[j] = 2 * j;
}

int main(int argc, char **argv)
{
	clock_t start = clock();

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
			result_id = shm_get("1.c", 'A');
			range_id = shm_get("1n.c", 'B');
			shm_attach(&results, result_id);
			shm_attach(&range, range_id);
			child_work(j);
			shm_detach(&results);
			shm_detach(&range);
			exit(0);
		default:
			printf("moj: %d, dziecka: %d\n", getpid(), pid);
		}
	}

	FILE *f = fopen("vector.dat", "r");
	char buffor[BUFFOR_SIZE + 1];
	double *vector;
	int n;

	// fgets(buffor, BUFFOR_SIZE, f);
	// n = atoi(buffor);
	// vector = malloc(sizeof(double) * n);
	// printf("Vector has %d elements\n", n);
	// for (i = 0; i < n; i++)
	// {
	// 	fgets(buffor, BUFFOR_SIZE, f);
	// 	vector[i] = atof(buffor);
	// }
	// fclose(f);

	result_id = shm_get("1.c", 'A');
	shm_attach(&results, result_id);
	range_id = shm_get("1n.c", 'B');
	shm_attach(&range, range_id);

	sleep(1);
	for (j = 0; j < N; j++)
		kill(children[j], SIGUSR1);
	for (j = 0; j < N; j++)
		if (wait(0) == -1)
		{
			perror("wait: wait failed [-1 dawg]");
			exit(1);
		}
	for (j = 0; j < N; j++)
		printf("%d ", results[j]);

	shm_detach(&range);
	shm_remove(range_id);
	shm_detach(&results);
	shm_remove(result_id);
	printf("\n%d %d \n", range_id, result_id);
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Czas trwania: %f \n", seconds);
	// free(vector);
	free(children);
	return 0;
}
/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define quantityOfProcess 10

struct time
{
	int hours;
	int minutes;
};
typedef struct time Time;

struct process
{
	Time time;
	int copies;
	int priority;
	char name[64];
};
typedef struct process Process;

int main(int argc, char *argv[])
{
	Process processReadyPriorityOne[quantityOfProcess];
	Process processReadyPriorityTwo[quantityOfProcess];
	Process processReadyPriorityThree[quantityOfProcess];
	Process processWainting[quantityOfProcess];
	
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        //O código aqui dentro será executado no processo filho
        printf(" soon %d\n\n\n", getpid());
    }
    else
    {
        printf(" father %d\n\n\n", getpid());

    }

	return 0;
}
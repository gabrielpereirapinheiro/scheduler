/*
	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
	Ismael Cordeiro - ismael@gmail.com
	University of Brasilia - 2018
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define quantityOfProcess 10

typedef struct Process
{
	char time[5];
	int copies;
	int priority;
	char name[15];
}process;

int main(int argc, char *argv[ ])
{
	process processReadyPriorityOne[quantityOfProcess];
	
	process processReadyPriorityTwo[quantityOfProcess];

	process processReadyPriorityThree[quantityOfProcess];

	process processWainting[quantityOfProcess];
	
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        //O código aqui dentro será executado no processo filho
        printf(" soon \n\n%d\n", getpid());
    }
    else
    {
        printf(" father %d\n\n\n", getpid());

    }


    
	return 0;
}
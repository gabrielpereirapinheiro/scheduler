/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define quantityOfProcess 2

struct time
{
	int hours;
	int minutes;
};
typedef struct time Time;

struct job
{
	int id;
	Time time;
	int copies;
	int priority;
	char name[64];
};
typedef struct job Job;

struct message
{
	long mType;
	Job job;
};
typedef struct message Message;


int main(int argc, char *argv[])
{
	// Job processReadyPriorityOne[quantityOfProcess];
	// Job processReadyPriorityTwo[quantityOfProcess];
	// Job processReadyPriorityThree[quantityOfProcess];
	 Job processWainting[quantityOfProcess];
	 int vetorpids[quantityOfProcess];

    int mqid;		// Message queue ID
    int id = 0;
    int pid ;//TESTE

    for(int i = 0; i < quantityOfProcess; i++)
    {
      if((pid = fork()) < 0)
      {
         printf("\n*Erro no fork\n");
         exit(1);
      }
      if(pid == 0)
      {
         vetorpids[i] = getpid();
         break;
      }
   }

	if ((mqid = msgget(0x1223, IPC_CREAT|0x1B6)) < 0) {
		printf("Error on message queue creation!! This program will be closed.\n");
		exit(1);
	}

    while(1)
    {
        Message message;
	    msgrcv(mqid, &message, sizeof(message) - sizeof(long), 0, 0);
	    Job job = message.job;
	    job.id = id;
        processWainting[id] = job;
	    id++;
	    printf("The name is: %s\n", job.name);
	    printf("The ID is: %d\n", job.id);
	    printf("Time:  %.2d:%.2d\n", job.time.hours, job.time.minutes);
	    printf("Priority: %d\n", job.priority);
	    printf("Number of copies: %d\n\n", job.copies);


       if((execl(job.name, job.name, NULL) < 0))
       {
           printf("ERROR: Ilegal file name ");
           exit(1);
        }
    }

    // pid_t pid;

    // if ((pid = fork()) < 0)
    // {
    //     perror("fork");
    //     exit(1);
    // }

    // if (pid == 0)
    // {
    //     //O código aqui dentro será executado no processo filho
    //     printf(" soon %d\n\n\n", getpid());
    // }
    // else
    // {
    //     printf(" father %d\n\n\n", getpid());

    // }

	return 0;
}
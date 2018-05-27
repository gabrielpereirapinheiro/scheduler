 /**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

using std::cout;
using std::endl;

struct job
{
	int id;
	int delay;
	int copies;
	int priority;
	char name[64];
};
typedef struct job Job;

struct message
{
	long pid;
	Job job;
};
typedef struct message Message;

/**
 * Function to process arguments received from terminal.
 */
int processArguments(int nArgs, char *args[], Job* job);

int main(int argc, char *argv[])
{
	Job job;

	//Se a funcao retornar menos -1 significa que ha falha nos argumentos
	if (processArguments(argc, argv, &job) == -1) {
		printf("Something went wrong!! This program will be closed.\n");
		exit(1);
	}

    //Criando memoria compartilhada
	int idshm;	
	if ((idshm = shmget(0x1223, sizeof(int), IPC_CREAT|0x1FF)) < 0) {
		printf("Error while creating the shared memory!! This program will be closed.\n");
		exit(1);
	}

	int *pshm;
	if ((pshm = (int*) shmat(idshm, (char*)0, 0)) == (int*)-1) {
		printf("Error while atraching to memory!! This program will be closed.\n ");
		exit(1);
	}
	*pshm += 1;
	job.id = *pshm;
	shmdt((char*)0);

    //Troca de mensagem entre solicita e executa
	int mqid;		// Message queue ID
	if ((mqid = msgget(0x1225, IPC_CREAT|0x1B6)) < 0) {
		printf("Error on message queue creation!! This program will be closed.\n");
		exit(1);
	}

	//Exibe numero do ID do job
	cout << "ID >> " << mqid << endl;

	Message message;
	message.pid = getpid();
	message.job = job;

	//Envia mensagem para o executa
	if ((msgsnd(mqid, &message, sizeof(message) - sizeof(long), 0)) < 0) {
		printf("Error na hora enviar a msg\n");
		exit(1);
	}

    //Mostra informacoes do job
	printf("The ID is: %d\n", job.id);
	printf("The name is: %s\n", job.name);
	printf("Delay:  %ds\n", job.delay);
	printf("Priority: %d\n", job.priority);
	printf("Number of copies: %d\n", job.copies);
	exit(0);
}

int processArguments(int nArgs, char *args[], Job* job)
{
	if (nArgs != 4 && nArgs != 5)
	{
		printf("ARGUMENT ERROR: Missing arguments\n");
		return -1;
	}

	// Define 1st argument: time
	char timeStrLength = strlen(args[1]);
	if (timeStrLength != 4 && timeStrLength != 5)
	{
		printf("ARGUMENT ERROR: Failed while parsing time string\n");
		return -1;
	}
	// Treat time string in the following format:
	// - h:MM
	// - hh:MM
	if (timeStrLength != 5)
	{
		char hoursStr[2];
		hoursStr[0] = args[1][0];
		hoursStr[1] = '\0';
		char minutesStr[3];
		strncpy(minutesStr, &args[1][2], 2);
		minutesStr[2] = '\0';
		job->delay = atoi(minutesStr) * 60 + atoi(hoursStr) * 3600;
	}
	else
	{
		char hoursStr[3];
		strncpy(hoursStr, args[1], 2);
		hoursStr[2] = '\0';
		char minutesStr[3];
		strncpy(minutesStr, &args[1][3], 2);
		minutesStr[2] = '\0';
		job->delay = atoi(minutesStr) * 60 + atoi(hoursStr) * 3600;
	}

	// Check if "copies" argument is bigger than zero.
	int copies = atoi(args[2]);
	if (copies < 1)
	{
		printf("ARGUMENT ERROR: Ilegal number of copies\n");
		return -1;
	}
	job->copies = copies;

	// Verify if priority was provided (nArgs = 5). If not,
	// set a default value.
	if (nArgs == 4)
	{
		// Copy string to another string. The maximum size of this
		// parameter is 63 characters.
		strncpy(job->name, args[3], 63);

		// Default priority.
		job->priority = 1;
	}
	else
	{
		// Store infomed priority.
		job->priority = atoi(args[3]);
		if (job->priority < 1 || job->priority > 3)
		{
			printf("ARGUMENT ERROR: Ilegal priority\n");
			return -1;
		}

		// Copy string to another string. The maximum size of this
		// parameter is 63 characters.
		strncpy(job->name, args[4], 63);
	}

	return 0;
}
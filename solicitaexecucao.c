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
	if (processArguments(argc, argv, &job) == -1) {
		printf("Something went wrong!! This program will be closed.\n");
		exit(1);
	}

	int mqid;		// Message queue ID
	if ((mqid = msgget(0x1223, IPC_CREAT|0x1B6)) < 0) {
		printf("Error on message queue creation!! This program will be closed.\n");
		exit(1);
	}

	Message message;
	message.pid = getpid();
	message.job = job;
	if ((msgsnd(mqid, &message, sizeof(message) - sizeof(long), 0)) < 0) {
		printf("Error na hora enviar a msg\n");
	}

	printf("The name is: %s\n", job.name);
	printf("Time:  %.2d:%.2d\n", job.time.hours, job.time.minutes);
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
		job->time.hours = atoi(hoursStr);
		char minutesStr[3];
		strncpy(minutesStr, &args[1][2], 2);
		minutesStr[2] = '\0';
		job->time.minutes = atoi(minutesStr);
		//int quantityOfSeconds = (job->time.minutes *60) + (job->time.hours * 60*60);
	}
	else
	{
		char hoursStr[3];
		strncpy(hoursStr, args[1], 2);
		hoursStr[2] = '\0';
		job->time.hours = atoi(hoursStr);
		char minutesStr[3];
		strncpy(minutesStr, &args[1][3], 2);
		minutesStr[2] = '\0';
		job->time.minutes = atoi(minutesStr);
	    //int quantityOfSeconds = (job->time.minutes *60) + (job->time.hours * 60*60);
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
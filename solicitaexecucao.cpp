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

#define SHM_KEY      0x1223
#define MSGQ_ASK_KEY 0x1224

struct job
{
	int id;
	int delay;
	int copies;
	int priority;
	char name[64];
};
typedef struct job Job;

struct askMessage
{
	long pid;
	Job job;
};
typedef struct askMessage AskMessage;

/**
 * Processes arguments received from terminal.
 */
bool tryProcessArguments(int nArgs, char *args[], Job* job);

int main(int argc, char *argv[])
{
	Job job;
	if (!tryProcessArguments(argc, argv, &job)) {
		cout << "Something went wrong!! This program will be closed." << endl;
		exit(1);
	}

	// Using shared memory, increment job ID.
	int idshm;	
	if ((idshm = shmget(SHM_KEY, sizeof(int), IPC_CREAT|0x1FF)) < 0) {
		cout << "Error while creating the shared memory!! This program will be closed." << endl;
		exit(1);
	}
	int *pshm;
	if ((pshm = (int*) shmat(idshm, (char*)0, 0)) == (int*)-1) {
		cout << "Error while atraching to memory!! This program will be closed." << endl;
		exit(1);
	}
	*pshm += 1;
	job.id = *pshm;
	shmdt((char*)0);

	int mqid;
	if ((mqid = msgget(MSGQ_ASK_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	AskMessage message;
	message.pid = getpid();
	message.job = job;
	if ((msgsnd(mqid, &message, sizeof(message) - sizeof(long), 0)) < 0) {
		cout << "Error while sending the message." << endl;
		exit(1);
	}

	cout << "ID:        " << job.id << endl;
	cout << "Name:      " << job.name << endl;
	cout << "Delay:     " << job.delay << endl;
	cout << "Priority:  " << job.priority << endl;
	cout << "Copies:    " << job.copies << endl;
	exit(0);
}

bool tryProcessArguments(int nArgs, char *args[], Job* job)
{
	if (nArgs != 4 && nArgs != 5)
	{
		cout << "ARGUMENT ERROR: Missing arguments." << endl;
		return false;
	}

	// Define 1st argument: time
	char timeStrLength = strlen(args[1]);
	if (timeStrLength != 4 && timeStrLength != 5)
	{
		cout << "ARGUMENT ERROR: Failed while parsing time string." << endl;
		return false;
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
		cout << "ARGUMENT ERROR: Ilegal number of copies." << endl;
		return false;
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
			cout << "ARGUMENT ERROR: Ilegal priority." << endl;
			return false;
		}

		// Copy string to another string. The maximum size of this
		// parameter is 63 characters.
		strncpy(job->name, args[4], 63);
	}

	return true;
}
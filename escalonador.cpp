/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <csignal>
#include <unistd.h>

using std::signal;
using std::cout;
using std::stringstream;
using std::endl;
using std::string;
using std::list;

#define MSGQ_ASK_KEY       0x1223
#define MSGQ_REM_KEY       0x1224
#define MSGQ_READY_KEY     0x1225
#define MSGQ_LIST_REQ_KEY  0x1226
#define MSGQ_LIST_RES_KEY  0x1227
#define MSGQ_LIST_SIZE_KEY 0x1228
#define QUANTUM		       5

struct job
{
	int id;
	int delay;
	int copies;
	int priority;
	char name[64];
};
typedef struct job Job;

struct readyJob
{
	int pid;
	int job;
	char name[64];
	int priority;
	int counter;
	int orientation;
};
typedef struct readyJob ReadyJob;

struct askMessage
{
	long mType;
	Job job;
};
typedef struct askMessage AskMessage;

struct readyMessage
{
	long mType;
	ReadyJob job;
};
typedef struct readyMessage ReadyMessage;

struct removeMessage
{
    long mType;
    int id_job;
};
typedef struct removeMessage RemoveMessage;

struct listReqMessage
{
    long mType;
};
typedef struct listReqMessage ListReqMessage;

struct listResMessage
{
	long pid;
	Job queueDelayJob;
};
typedef struct listResMessage ListResMessage;

struct listSizeMessage
{
	long pid;
	int size;
};
typedef struct listSizeMessage ListSizeMessage;


void exec();
void listen();

int main(int argc, char *argv[])
{
	cout << "Starting scheduler..." << endl;

    pid_t pid;
    if ((pid = fork()) < 0)
    {
        cout << "Error on creation of processes listen and exec." << endl;
        exit(1);
	}

	// Exec is the parent process.
    (pid != 0) ? exec() : listen();
	return 0;
}

void exec()
{
	list<ReadyJob> priorityOne;
	list<ReadyJob> priorityTwo;
	list<ReadyJob> priorityThree;
	int status;

	int mqid;
	if ((mqid = msgget(MSGQ_READY_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	while (true) {

		ReadyMessage message;
		if (msgrcv(mqid, &message, sizeof(message) - sizeof(long), 0, IPC_NOWAIT) > -1) {

			pid_t pid;
			if ((pid = fork()) < 0){
				cout << "Error on creation of processes listen and exec." << endl;
				exit(1);
			}
			if (pid == 0) {

				// Execute job.
				if (execl(message.job.name, message.job.name, nullptr) < 0){
					cout << "Error on executing program." << endl;
					exit(1);
				}
				exit(0);

			} else {

				message.job.pid = pid;

				// Stop process right after it starts.
				kill(pid, SIGSTOP);

				// Add to its respective priority queue.
				switch(message.job.priority) {
					case 1:
						priorityOne.push_back(message.job);
						break;
					case 2:
						priorityTwo.push_back(message.job);
						break;
					default:
						priorityThree.push_back(message.job);
				}
			}
		}

		// Choose job to be executed.
		ReadyJob execute;
		if (!priorityOne.empty()) {
			execute = priorityOne.front();
			priorityOne.pop_front();

		} else if (!priorityTwo.empty()) {
			execute = priorityTwo.front();
			priorityTwo.pop_front();

		} else if (!priorityThree.empty()) {
			execute = priorityThree.front();
			priorityThree.pop_front();

		} else {
			continue;
		}

		// Re-start process execution.
		kill(execute.pid, SIGCONT);

		// Initialize timeCount.
		int timeCount = 0;

		// If the process finished before quantum is achieved,
		// set this flag.
		bool isFinished = false;

		// Wait until quantum.
		while (timeCount++ < QUANTUM) {

			int status, pid = waitpid(execute.pid, &status, WNOHANG);
			if (pid == -1) {
				cout << "wait() error." << endl;
			} else if (pid == 0) {
				sleep(1);
			} else {
				isFinished = true;
				break;
			}
			sleep(1);
		}

		if (isFinished) {
			continue;
		}

		// Stop process execution.
		kill(execute.pid, SIGSTOP);

		// Increment the process' execution counter.
		execute.counter++;

		// Re-calculate priority
		if (execute.counter >= 2) {
			execute.counter = 0;
			switch (execute.priority) {
				case 1:
					execute.orientation = 0;
					execute.priority = 2;
					break;

				case 2:
					execute.priority = (execute.orientation == 0) ? 3 : 1;
					break;

				default:
					execute.orientation = 1;
					execute.priority = 2;

			}
		}

		// Re-enter process on its current priority queue.
		switch (execute.priority) {
			case 1:
				priorityOne.push_back(execute);
				break;
			case 2:
				priorityTwo.push_back(execute);
				break;
			default:
				priorityThree.push_back(execute);
		}
	}

	// Wait for child processes to be finished.
	wait(&status);
}

void listen()
{
	list<Job> queueDelayJobs;

	int mqidAsk;
	if ((mqidAsk = msgget(MSGQ_ASK_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	int mqidRem;
	if ((mqidRem = msgget(MSGQ_REM_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	int mqidReady;
	if ((mqidReady = msgget(MSGQ_READY_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	int mqidListReq;
	if ((mqidListReq = msgget(MSGQ_LIST_REQ_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	int mqidListRes;
	if ((mqidListRes = msgget(MSGQ_LIST_RES_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	int mqidListSize;
	if ((mqidListSize = msgget(MSGQ_LIST_SIZE_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

	while (true) {

		AskMessage askMessage;
		if (msgrcv(mqidAsk, &askMessage, sizeof(askMessage) - sizeof(long), 0, IPC_NOWAIT) > -1) {
			// Push to the queue.
			queueDelayJobs.push_back(askMessage.job);
		}

		RemoveMessage remMessage;
		if (msgrcv(mqidRem, &remMessage, sizeof(remMessage) - sizeof(long), 0, IPC_NOWAIT) > -1) {

			// Remove from the queue.
			for (auto it = queueDelayJobs.begin(); it != queueDelayJobs.end(); it++) {
				if (it->id == remMessage.id_job) {
					queueDelayJobs.erase(it);
					break;
				}
			}
		}

		ListReqMessage listReqMessage;
		if (msgrcv(mqidListReq, &listReqMessage, sizeof(listReqMessage) - sizeof(long), 0, IPC_NOWAIT) > -1) {

			// Send first the size of the list.
			ListSizeMessage listSizeMessage;
			listSizeMessage.pid = getpid();
			listSizeMessage.size = queueDelayJobs.size();
			if ((msgsnd(mqidListSize, &listSizeMessage, sizeof(listSizeMessage) - sizeof(long), 0)) < 0) {
				cout << "Error while sending the message." << endl;
				exit(1);
			}

			// Send each element of the list.
            if (queueDelayJobs.size() > 0) {
				for (auto job : queueDelayJobs) {

					ListResMessage listResMessage;
					listResMessage.pid = getpid();
					listResMessage.queueDelayJob.id = job.id;
					listResMessage.queueDelayJob.copies = job.copies;
					listResMessage.queueDelayJob.delay = job.delay;
					listResMessage.queueDelayJob.priority = job.priority;
					strcpy(listResMessage.queueDelayJob.name, job.name);
					
					if ((msgsnd(mqidListRes, &listResMessage, sizeof(listResMessage) - sizeof(long), 0)) < 0) {
						cout << "Error while sending the message." << endl;
						exit(1);
					}
                }
			}
		}

		// Verifica os prontos na fila e cria copias processos e manda (PID, JID, Contador, Orientacao) para o processo EXEC.
		// Começa a executar o processo e imediatamente ele parado com SIGSTOP.

		list<list<Job>::iterator> jobsToBeRemoved;
		int mi = 0;
		for (auto it = queueDelayJobs.begin(); it != queueDelayJobs.end(); it++) {

			Job job = *it;
			if (job.delay <= 0) {
				mi++;

				jobsToBeRemoved.push_back(it);
				for (int i = 0; i < job.copies; i++){
					
					// Create a ready job to be sent to exec process.
					ReadyJob jobToSave;
					jobToSave.pid = -1;
					jobToSave.job = job.id;
					jobToSave.priority = job.priority;
					jobToSave.counter = 0;
					strcpy(jobToSave.name, job.name);

					// Define orientation to always go top-down.
					if(jobToSave.priority==2){
						jobToSave.orientation = 0;
					}

					ReadyMessage message;
					message.mType = getpid() + mi;
					message.job = jobToSave;
					if ((msgsnd(mqidReady, &message, sizeof(message) - sizeof(long), 0)) < 0) {
						cout << "Error while sending the message." << endl;
						exit(1);
					}
					cout << "Job #" << jobToSave.job << " sent to the queue #" << jobToSave.priority <<  endl;
				}
			}
		}

		// Remove all jobs that waited for delay time.
		for (auto remove : jobsToBeRemoved) {
			queueDelayJobs.erase(remove);
		}

		sleep(1);
		
		// Decrease remaining delay time.
		for (auto& job : queueDelayJobs) {
			job.delay--;
		}
	}
}
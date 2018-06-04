/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctime>
#include <unistd.h>

using std::cout;
using std::endl;

#define MSGQ_LIST_REQ_KEY  0x1226
#define MSGQ_LIST_RES_KEY  0x1227
#define MSGQ_LIST_SIZE_KEY 0x1228

struct job
{
	int id;
	int delay;
	int copies;
	int priority;
	char name[64];
};
typedef struct job Job;

struct listReqMessage
{
    long pid;
};
typedef struct listReqMessage ListReqMessage;

struct listResMessage
{
	long mType;
	Job queueDelayJob;
};
typedef struct listResMessage ListResMessage;

struct listSizeMessage
{
	long mType;
	int size;
};
typedef struct listSizeMessage ListSizeMessage;


int main(int argc, char *argv[])
{
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

	// Send request.
    ListReqMessage reqMessage;
    reqMessage.pid = getpid();
	if ((msgsnd(mqidListReq, &reqMessage, sizeof(reqMessage) - sizeof(long), 0)) < 0) {
		cout << "Error while sending the message." << endl;
		exit(1);
	}

	// Wait for size response.
	ListSizeMessage sizeMessage;
	msgrcv(mqidListSize, &sizeMessage, sizeof(sizeMessage) - sizeof(long), 0, 0);
	int size = sizeMessage.size;
	
	// Show each delay job.
	if (size < 1) {
		cout << endl << "List empty\n" << endl;
	} else {
		cout << endl << "Job         arq_exec  hhmm  copias  pri" << endl;
		cout << "---------------------------------------" << endl;
		while (size-- > 0) {
			ListResMessage resMessage;
			msgrcv(mqidListRes, &resMessage, sizeof(resMessage) - sizeof(long), 0, 0);

			auto now = time(0);
			auto local = localtime(&now);
			int minutes = local->tm_min + resMessage.queueDelayJob.delay / 60;
			int hours = local->tm_hour + minutes / 60;

			printf("%3d", resMessage.queueDelayJob.id);
			printf("  %15s", resMessage.queueDelayJob.name);
			printf(" %.2d:%.2d", hours % 24, minutes % 60);
			printf("  %6d", resMessage.queueDelayJob.copies);
			printf("  %3d", resMessage.queueDelayJob.priority);
			cout << endl;
		}
		cout << "---------------------------------------" << endl << endl;
	}

    exit(0);
}


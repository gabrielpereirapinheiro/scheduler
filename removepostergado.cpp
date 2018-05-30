/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

using std::cout;
using std::endl;

#define MSGQ_REM_KEY 0x1224

struct removeMessage
{
    long pid;
    int id_job;
};
typedef struct removeMessage RemoveMessage;

/**
 * Processes the arugments received from terminal.
 */
bool tryProcessArguments(int nArgs, char *args[], int* id_job);

int main(int argc, char *argv[])
{
    int id_job;

    if (!tryProcessArguments(argc, argv, &id_job)) {
        cout << "Something went wrong!! This program will be closed." << endl;
        exit(1);
    }

    int mqid;
	if ((mqid = msgget(MSGQ_REM_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

    RemoveMessage message;
    message.pid = getpid();
    message.id_job = id_job;
	if ((msgsnd(mqid, &message, sizeof(message) - sizeof(long), 0)) < 0) {
		cout << "Error while sending the message." << endl;
		exit(1);
	}

    cout << "Removal request sent successfully to Job ID: " << id_job << endl;
    exit(0);
}

bool tryProcessArguments(int nArgs, char *args[], int* id_job)
{
    if (nArgs != 2)
    {
        cout << "ARGUMENT ERROR: Missing arguments" << endl;
        return false;
    }

    *id_job = atoi(args[1]);
    return true;
}
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

#define MSGQ_REM_KEY 0x1227


struct listProcess
{
    long pid;
};
typedef struct listProcess ListProcess;



int main(int argc, char *argv[])
{
    int mqid;
	if ((mqid = msgget(MSGQ_REM_KEY, IPC_CREAT|0x1B6)) < 0) {
		cout << "Error on message queue creation!! This program will be closed." << endl;
		exit(1);
	}

    ListProcess message;
    message.pid = getpid();

	if ((msgsnd(mqid, &message, sizeof(message) - sizeof(long), 0)) < 0) {
		cout << "Error while sending the message." << endl;
		exit(1);
	}

    cout << "All process was listed" << endl;

    exit(0);
}


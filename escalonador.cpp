/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <iostream>
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <csignal>
#include <unistd.h>

#define quantityOfProcess 2

using std::signal;
using std::cout;
using std::endl;
using std::list;

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
	int priority;
	int counter;
	//1 to up and 0 to down
	int orientation;
};
typedef struct readyJob ReadyJob;

struct readyMessage
{
	long mType;
	ReadyJob job;
};
typedef struct readyMessage ReadyMessage;

struct message
{
	long mType;
	Job job;
};
typedef struct message Message;

int isDead = false;

void handleChildDeath(int status);

int main(int argc, char *argv[])
{
	// Job processReadyPriorityOne[quantityOfProcess];
	// Job processReadyPriorityTwo[quantityOfProcess];
	// Job processReadyPriorityThree[quantityOfProcess];
	//Job processWaiting[quantityOfProcess];

	signal(SIGUSR1, handleChildDeath);

	cout << "Vou ser clonado!" << endl;
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        printf("Error on creation of processes listen and exec");
        exit(1);
    }	

	cout << "Estou clonado!" << endl;

    //Escalonador parte executa
    if (pid != 0)
    {
        //Inicia as tres filas de prioridade
		list<ReadyJob> priorityOne;
		list<ReadyJob> priorityTwo;
		list<ReadyJob> priorityThree;

		int mqid;		// Message queue ID
		if ((mqid = msgget(0x1226, IPC_CREAT|0x1B6)) < 0) {
			printf("Error on message queue creation!! This program will be closed.\n");
			exit(1);
		}

		cout << "estou no processo pai" << endl;
		int i;
		while (1) {

			ReadyMessage message;
			if (msgrcv(mqid, &message, sizeof(message) - sizeof(long), 0, IPC_NOWAIT) > -1) {
				cout << "recebeu mensagem" << endl;
				if (message.job.priority == 1) {
					priorityOne.push_back(message.job);
				} else if (message.job.priority == 2) {
					priorityTwo.push_back(message.job);
				} else {
					priorityThree.push_back(message.job);	
				}
			}

			// Escolhe job a ser executado seguindo a prioridade

            ReadyJob execute;
			execute.pid = -1;

            //Ve lista 1
            if(priorityOne.empty())
            {
                // se ela for vazia ja tenta olhar na lista dois

                if(priorityTwo.empty()){
                    if(priorityThree.empty()){
                    }
                    else{
                        //Se nao pega o primeiro item da lista
                        execute = priorityThree.front();
                        priorityThree.pop_front();
                    }
                }
                else{
                    //Se nao pega o primeiro item da lista
                    execute = priorityTwo.front();
                    priorityTwo.pop_front();
                }
            }
            else{
                //Se nao pega o primeiro item da lista
                execute = priorityOne.front();
                priorityOne.pop_front();
            }

			if (execute.pid == -1) {
				continue;
			}

			// Re-começa a execução do processo
			cout << "Counter: " << execute.counter << " | PID: " << execute.pid << " | JID: " << execute.job << " | P: " << execute.priority << " | O: " << execute.orientation << endl;
            kill(execute.pid, SIGCONT);

			i = 0;

			int isFinished = 0;
			while (i++ < 5) {

				if (isDead) {
					// Processo acabou
					isFinished = 1;
					break;
				}
				sleep(1);
			}

			// if (isFinished == 1) {
			// 	continue;
			// }

			// Para a execução do processo
            kill(execute.pid, SIGSTOP);

            //Para indicar que ela ja esta nessa priorirdade
            execute.counter++;

			// Recalcula a prioridade
			if(execute.counter==2){
				execute.counter = 0;
                if(execute.priority==1){
                    execute.orientation=0;
					execute.priority = 2;
                    priorityTwo.push_back(execute);
                }
                if(execute.priority==2){
                    if(execute.orientation==1){
						execute.priority = 1;
                        priorityOne.push_back(execute);
                    }
                    else{
						execute.priority = 3;
                        priorityThree.push_back(execute);
                    }
                }
                if(execute.priority==3){
                    execute.orientation=1;
					execute.priority = 2;
                    priorityTwo.push_back(execute);
                }
			}
			//Coloca de volta na fila
			else{
			    if(execute.priority==1){
			        priorityOne.push_back(execute);
			    }
			    else if(execute.priority==2){
			        priorityTwo.push_back(execute);
			    }
			    else{
			        priorityThree.push_back(execute);
			    }
			}

			execute.pid = -1;
		}
    }
        //Escalonador parte que escuta

    else{
		list<Job> queueDelayJobs;

    	int mqid;		// Message queue ID
		if ((mqid = msgget(0x1225, IPC_CREAT|0x1B6)) < 0) {
			printf("Error on message queue creation!! This program will be closed.\n");
			exit(1);
		}

		int mqidReady;		// Message queue ID
		if ((mqidReady = msgget(0x1226, IPC_CREAT|0x1B6)) < 0) {
			printf("Error on message queue creation!! This program will be closed.\n");
			exit(1);
		}

        while(1){
			Message message;

			if (msgrcv(mqid, &message, sizeof(message) - sizeof(long), 0, IPC_NOWAIT) > -1) {
				
				// Coloca na fila
				queueDelayJobs.push_back(message.job);

				Job job = message.job;
				cout << "The ID is: " << job.id << endl;
				cout << "The name is: " << job.name << endl;
				cout << "Delay: " << job.delay << "s" << endl;
				cout << "Priority: " << job.priority << endl;
				cout << "Number of copies: " << job.copies << endl;
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
					int ppid = getpid();
					for (int i = 0; i < job.copies; i++){
						pid_t pidExec;
						if ((pidExec = fork()) < 0){
							printf("Error on creation of processes listen and exec");
							exit(1);
						}

						if (pidExec == 0) {
							if (execl(job.name, job.name, nullptr) < 0){
								cout << "Error on executing program" << endl;
								exit(1);
							}
							kill(ppid, SIGUSR1);
							exit(0);

						} else {

							// Pausar o processo logo depois dele ter sido criado.
							kill(pidExec, SIGSTOP);

							//Cria um ReadyJob para salvar na fila de prioridade correta
                        	ReadyJob jobToSave;
                            jobToSave.pid = pidExec;
                            jobToSave.job = job.id;
							jobToSave.priority = job.priority;
                            jobToSave.counter = 0;

                            //Definir que sempre desce primeiro
                            if(jobToSave.priority==2){
                                jobToSave.orientation = 0;
                            }

							ReadyMessage message;
							message.mType = getpid() + mi;
							message.job = jobToSave;
							if ((msgsnd(mqidReady, &message, sizeof(message) - sizeof(long), 0)) < 0) {
								printf("Error na hora enviar a msg\n");
								exit(1);
							}
                			cout << "Send to queue job " << jobToSave.job << endl;
						}
					}
				}
			}

			for (auto remove : jobsToBeRemoved) {
				queueDelayJobs.erase(remove);
			}

			sleep(1);
			
			// Decrementa delay nos elementos da fila
			for (auto& job : queueDelayJobs) {
		       cout << "The ID is: " << job.id << " Delay "<<job.delay << endl;
				job.delay--;
			}
		}
    }

	return 0;
}

void handleChildDeath(int status)
{
	isDead = true;
}
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


struct readyJob
{
	int pid;
	int job;
	int counter;
	//1 to up and 0 to down
	int orientation;
};

typedef struct job Job;

struct message
{
	long mType;
	Job job;
};
typedef struct message Message;

int isDead = 0;

void handleChildDeath(int status);

int main(int argc, char *argv[])
{
	// Job processReadyPriorityOne[quantityOfProcess];
	// Job processReadyPriorityTwo[quantityOfProcess];
	// Job processReadyPriorityThree[quantityOfProcess];
	//Job processWaiting[quantityOfProcess];

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
		list<readyJob> priorityOne;

		list<readyJob> priorityTwo;

		list<readyJob> priorityThree;

		cout << "estou no processo pai" << endl;
		int i;
		while (1) {
			// Escolhe job a ser executado seguindo a prioridade

            readyJob execute;

            int level = -1;
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
                        level = 3;
                        priorityTwo.pop_front();
                    }
                }
                else{
                    //Se nao pega o primeiro item da lista
                    execute = priorityTwo.front();
                    level = 2;
                    priorityThree.pop_front();
                }
            }
            else{
                //Se nao pega o primeiro item da lista
                execute =  priorityOne.front();
                level = 1;
                priorityOne.pop_front();
            }

			// Re-começa a execução do processo
			//kill(SIGCONT, pid);
            //kill(SIGCONT, execute.pid)

			// Registra rotina de tratamento do para a morte do filho
			signal(SIGCHLD, handleChildDeath);

			i = 0;

            //Para indicar que ela ja esta nessa priorirdade
            execute.counter++;

			while (i++ < 5) {
				if (isDead) {
					// Remove job da fila  EU JA REMOVI QUANDO EU PEGO ELE PARA EXECUTAR
					break;	
				}
				sleep(1);
			}

			// Para a execução do processo
			//kill(SIGSTOP, pid);
            //kill(SIGSTOP, execute.pid);

			// Recalcula a prioridade
			if(execute.counter==2){
                if(level==1){
                    execute.orientation=0;
                    priorityTwo.push_back(execute);
                }
                if(level==2){
                    if(execute.orientation==1){
                        priorityOne.push_back(execute);
                    }
                    else{
                        priorityThree.push_back(execute);
                    }
                }
                if(level==3){
                    execute.orientation=1;
                    priorityTwo.push_back(execute);
                }
			}
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
		cout << "ID >> " << mqid << endl;

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
			for (auto job : queueDelayJobs) {
				if (job.delay <= 0) {

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
							exit(0);

						} else {

							// Pausar o processo logo depois dele ter sido criado.
							kill(pidExec, SIGSTOP);
                            readyJob jobToSave;
                            jobToSave.pid = pidExec;
                            jobToSave.job = job.id;
                            jobToSave.counter = 0;

                            //Definir que sempre desce primeiro
                            if(job.priority==2){
                                jobToSave.orientation = 0;
                            }
                			cout << "Send to queue job " << jobToSave.job<< endl;

							// manda (PID, JID, Contador, Orientacao)
						}
					}
				}
			}

			sleep(1);
			
			// Decrementa delay nos elementos da fila
			for (auto& job : queueDelayJobs) {
		       //cout << "The ID is: " << job.id << " Delay "<<job.delay << endl;
				job.delay--;
			}
		}
    }

	return 0;
}

void handleChildDeath(int status) {
	isDead = 1;
}
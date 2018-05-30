Necessario:

G++ instalado (versao utilizada no desenvolvimento 5.4.0) 

Como executar o programa:

Acesse a pasta scheduler do projeto pelo terminal e utilize o comando :

$ make

e em seguid, use o comando

$ ./escalonador &

para inicializar o escanolador.
Para executar solicita_execucao, use:

$ ./solicita_execucao <HH:MM> <C> <P> <NAME>

Onde:

HH:MM é o delay. Exemplo: 02:21, o programa irá executar apos 2 horas e 21 minutos depois.

C é um numero inteiro maior ou igual a 0 que representa o numero de copias.

P é um numero inteiro podendo ser 1,2 ou 3 e representa a prioridade daquele processo (1>2>3).

Name é o nome do arquivo executavel e precisa estar dentro da pasta do projeto (scheduler)

Para executar o remove_postergado, use o comando:

$ ./remove_postergado <job_id>

Onde, job_id é o numero do job que deseja retirar da fila de postegado.

Para executar o lista_postergado, use o comando:

$ ./lista_postergado
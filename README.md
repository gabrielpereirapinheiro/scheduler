# Sheduler
Project of operational systems (OS)

### About
This project is an implementation of a ***staggered process scheduling manager*** and was build and can be can be build using `make` or using `gcc` on both files:
- escalonador.c
- solicitaexecucao

### Execute

To execute, inside the path of project `scheduler` use the command in terminal :

> make

And now, use the command :

> ./escalonador

Now, you can open a new terminal, and use the commnad :

> ./solicitaexecucao `HH:MM` `C` `P` `NAME`

Where:

`HH:MM` it's the delay, example : 01:15, the program will wait for 1 hour and 35 minutes and after that will be executed. So the formart is the firts two digits represent hour and the last two minutes.

`C` is an intenger number gratter thant 0 and represent the number of copies.

`P` is an integer number, can be 1, 2 or 3.

`NAME` is the name of executable file, this file needs to be inside the project folder.


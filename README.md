# Sheduler

### About
This project is an implementation of a ***staggered process scheduling manager*** and was build and can be can be build using `make` or using `g++` on both files:
- escalonador.cpp
- solicitaexecucao.cpp
- removepostergado.cpp
- listaportergado.cpp

The programming langugage used was `C++` so is necessary have the `g++` compiler installed in you computer. We've used C++11 on this project, so to compile each file, we use the flag `-std=c++11`.

We used the version 5.4.0 of g++.

### Execute

To compile the files of this project, inside the path of project `scheduler`, use the command:
```
 $ make
```

To execute the `escalonador`, use the command:
```
$ ./escalonador &
```

To execute the `solicita_execucao`, use the command:
```
$ ./solicita_execucao <HH:MM> <C> <P> <NAME>
```
Where:

- `HH:MM` it's the delay, example : 01:15, the program will wait for 1 hour and 35 minutes and after that will be executed. So the formart is the firts two digits represent hour and the last two minutes.

- `C` is an intenger number gratter thant 0 and represent the number of copies.

- `P` is an integer number, can be 1, 2 or 3 and represent the priority. (1 is the bigger and 3 the smaller).

- `NAME` is the name of executable file, this file needs to be inside the project folder.


To execute the `remove_postergado`, use the command:
```
$ ./remove_postergado <job_id>
```

To execute the `lista_postergado`, use the ocmmand:
```
$ ./lista_postergado
```



## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/gabrielpereirapinheiro/scheduler/blob/master/LICENSE) file for details


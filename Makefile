GCC = @g++ -std=c++11 -Wall -g
RM = @rm -rf

all:
	@echo "Compila solicitaexecucao"
	$(GCC) solicitaexecucao.cpp -o solicitaexecucao
	@echo "Compila escalonador"
	$(GCC) escalonador.cpp -o escalonador

clean:
	$(RM) solicitaexecucao
	$(RM) escalonador
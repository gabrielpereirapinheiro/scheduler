GCC = @gcc -Wall -g
RM = @rm -rf

all:
	@echo "Compila solicitaexecucao"
	$(GCC) solicitaexecucao.c -o solicitaexecucao
	@echo "Compila escalonador"
	$(GCC) escalonador.c -o escalonador

clean:
	$(RM) solicitaexecucao
	$(RM) escalonador
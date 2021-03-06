GCC = @g++ -std=c++11 -Wall -g
RM = @rm -rf

all:
	@echo "Compila solicita_execucao"
	$(GCC) solicitaexecucao.cpp -o solicita_execucao
	@echo "Compila remove_postergado"
	$(GCC) removepostergado.cpp -o remove_postergado
	@echo "Compila escalonador"
	$(GCC) escalonador.cpp -o escalonador
	@echo "Compila lista postergado"
	$(GCC) listapostergados.cpp -o lista_postergados

clean:
	$(RM) solicita_execucao
	$(RM) remove_postergado
	$(RM) escalonador
	$(RM) lista_postergados
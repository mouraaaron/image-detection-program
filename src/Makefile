CC = gcc
FLAGS = -Wall -Wextra

tudo: ../objeto

../objeto: ../bin/objetos.o ../bin/imagens.o ../bin/deteccao.o ../bin/fila.o ../bin/funcoes_aux.o
	$(CC) $(FLAGS) -o ../objeto ../bin/objetos.o ../bin/imagens.o ../bin/deteccao.o ../bin/fila.o ../bin/funcoes_aux.o

../bin/objetos.o: objetos.c
	$(CC) $(FLAGS) -c objetos.c -o ../bin/objetos.o

../bin/imagens.o: imagens.c
	$(CC) $(FLAGS) -c imagens.c -o ../bin/imagens.o

../bin/deteccao.o: deteccao.c
	$(CC) $(FLAGS) -c deteccao.c -o ../bin/deteccao.o

../bin/fila.o: fila.c
	$(CC) $(FLAGS) -c fila.c -o ../bin/fila.o

../bin/funcoes_aux.o: funcoes_aux.c
	$(CC) $(FLAGS) -c funcoes_aux.c -o ../bin/funcoes_aux.o

limpa:
	rm -f ../bin/*.o

faxina: limpa
	rm -f ../objeto
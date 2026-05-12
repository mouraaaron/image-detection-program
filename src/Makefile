CC     = gcc
FLAGS  = -Wall -Wextra -std=c99

OBJS   = ../bin/objetos.o \
         ../bin/imagens.o \
         ../bin/deteccao.o \
         ../bin/fila.o \
         ../bin/funcoes_aux.o

tudo: $(OBJS)
	$(CC) $(FLAGS) -o ../bin/objeto $(OBJS)
	cp ../bin/objeto ../objeto

../bin/objetos.o: objetos.c imagens.h deteccao.h fila.h funcoes_aux.h
	$(CC) $(FLAGS) -c objetos.c -o ../bin/objetos.o

../bin/imagens.o: imagens.c imagens.h fila.h funcoes_aux.h
	$(CC) $(FLAGS) -c imagens.c -o ../bin/imagens.o

../bin/deteccao.o: deteccao.c deteccao.h imagens.h fila.h funcoes_aux.h
	$(CC) $(FLAGS) -c deteccao.c -o ../bin/deteccao.o

../bin/fila.o: fila.c fila.h
	$(CC) $(FLAGS) -c fila.c -o ../bin/fila.o

../bin/funcoes_aux.o: funcoes_aux.c funcoes_aux.h imagens.h
	$(CC) $(FLAGS) -c funcoes_aux.c -o ../bin/funcoes_aux.o

limpa:
	rm -f ../bin/*.o

faxina: limpa
	rm -f ../bin/objeto ../objeto

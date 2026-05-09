#ifndef FILA_H
#define FILA_H
#include <stdio.h>


typedef struct {
    int *linhas;
    int *colunas;
    int inicio;
    int fim;
} Fila; //vai ser usado para o flood fill 

Fila *criarFila(int tamanho);

void enfileirar(Fila *fila, int linha, int coluna);

void desenfileirar(Fila *fila, int *linha, int *coluna);

int filaVazia(Fila *fila);

void liberarFila(Fila *fila);

#endif
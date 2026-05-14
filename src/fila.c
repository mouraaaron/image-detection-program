#include "fila.h"

#include <stdio.h>
#include <stdlib.h>

Fila *criarFila(int tamanho) {
    Fila *fila = malloc(sizeof(Fila));
    fila->linhas = malloc(tamanho * sizeof(int));
    fila->colunas = malloc(tamanho * sizeof(int));
    fila->inicio = 0;
    fila->fim = 0;
    return fila;
}

void enfileirar(Fila *fila, int linha, int coluna) {
    fila->linhas[fila->fim] = linha;
    fila->colunas[fila->fim] = coluna;
    fila->fim++;
}

void desenfileirar(Fila *fila, int *linha, int *coluna) {
    *linha = fila->linhas[fila->inicio];
    *coluna = fila->colunas[fila->inicio];
    fila->inicio++;
}

int filaVazia(Fila *fila) { return fila->inicio == fila->fim; }

void liberarFila(Fila *fila) {
    free(fila->linhas);
    free(fila->colunas);
    free(fila);
}
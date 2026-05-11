#ifndef DETECCAO_H
#define DETECCAO_H
#include <stdio.h>
#include "imagens.h"
#include "fila.h"


typedef struct {
    int id;
    int x;
    int y;
    float area;
    int minLinha, maxLinha, minColuna, maxColuna;
    int larguraBBox, alturaBBox;
    int **mascara;
} Objeto;

typedef struct {
    int **rotulos;
    Objeto *objetos;
    int totalObjetos;
    int *corDoObjeto;
} Deteccao;


Deteccao *detectarObjetos(Imagem *img);

void liberarDeteccao(Deteccao *det, int altura);

int floodFill(Imagem *img, int **rotulos, int linhaInicial, int colunaInicial, int id,
              int *minLinha, int *maxLinha, int *minColuna, int *maxColuna);

void gerarImagemSaida(Imagem *img, Deteccao *det, const char *nomeArquivo);

void gerarImagemMaioresObjetos(Imagem *img, Deteccao *det, const char *nomeArquivo);

void imprimirListaOrdenada(Deteccao *det);


#endif
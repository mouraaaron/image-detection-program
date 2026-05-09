#ifndef DETECCAO_H
#define DETECCAO_H
#include <stdio.h>
#include "imagens.h"
#include "fila.h"


typedef struct {
    int id;
    int x;        
    int y;        
    int area;
} Objeto;

typedef struct {
    int **rotulos;   
    Objeto *objetos; 
    int totalObjetos;
} Deteccao;


Deteccao *detectarObjetos(Imagem *img);

void liberarDeteccao(Deteccao *det, int altura);

int floodFill(Imagem *img, int **rotulos, int linhaInicial, int colunaInicial, int id);

void gerarImagemSaida(Imagem *img, Deteccao *det, const char *nomeArquivo);


#endif
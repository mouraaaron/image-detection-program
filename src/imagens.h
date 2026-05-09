#ifndef IMAGENS_H
#define IMAGENS_H
#include <stdio.h>


typedef struct {
    int largura;
    int altura;
    int valorMax;
    int **pixels;
} Imagem;




Imagem *abrirImagem(const char *caminho);

void liberarImagem(Imagem *imagem);


#endif
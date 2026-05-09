#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagens.h"
#include "fila.h"
#include "funcoes_aux.h"





Imagem *abrirImagem(const char *nomeImagem)
{
    FILE *arquivo = fopen(nomeImagem, "rb");
    if(!arquivo)
    {
        fprintf(stderr, "Erro ao abrir a arquivo.\n");
        return NULL;
    }   

    char tipo[3];
    fscanf(arquivo, "%2s", tipo);
    printf("Tipo da imagem: %s\n", tipo);
    if(strcmp(tipo, "P5") != 0 && strcmp(tipo, "P2") != 0)
    {
        fprintf(stderr, "apenas formato p5 ou p2 suportados.\n");
        fclose(arquivo);
        return NULL;
    }

    //temos que ignorar os comentários que começam com #

    char linha[256];
    fgets(linha, 256, arquivo);
    while(linha[0] == '#')
        fgets(linha, 256, arquivo);

    Imagem *img = malloc(sizeof(Imagem)); //struct c informacoes da imagem
    sscanf(linha, "%d %d", &img->largura, &img->altura);
    fscanf(arquivo, "%d", &img->valorMax);
    //    fgetc(arquivo);  // consome o '\n' após maxval

    img->pixels = malloc(img->altura * sizeof(int *));
    for(int i = 0; i < img->altura; i++)
        img->pixels[i] = malloc(img->largura * sizeof(int));

    if(strcmp(tipo, "P5") == 0)
    {
        for(int i = 0; i < img->altura; i++)
        {
            for(int j = 0; j < img->largura; j++)
            {
                if(img->valorMax < 256)
                    img->pixels[i][j] = fgetc(arquivo);
                else
                {
                    int bytemaisimportante = fgetc(arquivo);
                    int bytemenosimportante = fgetc(arquivo);
                    img->pixels[i][j] = (bytemaisimportante * 256 + bytemenosimportante);
                }
            }
                
        }
    }
    else
    {
        for(int i = 0; i < img->altura; i++)
        {
            for(int j = 0; j < img->largura; j++)
            {
                fscanf(arquivo, "%d", &img->pixels[i][j]);
            }
        }
    }

    fclose(arquivo);
    return img;

}

void liberarImagem(Imagem *imagem)
{
    for(int i = 0; i < imagem->altura; i++)
        free(imagem->pixels[i]);
    free(imagem->pixels);
    free(imagem);
}






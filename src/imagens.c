#include "imagens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fila.h"
#include "funcoes_aux.h"

// avança o ponteiro do arquivo ignorando espaços em branco e linhas de comentário (#) 
void skipEspacosEComentarios(FILE *f) {
    int c;
    int ehEspaco;

    c = fgetc(f);
    while (c != EOF) //EOF = End of FIle 
        {
        if (c == '#')
            {
            while (c != '\n' && c != EOF)
                {
                c = fgetc(f);
                }

        } else 
        {
            ehEspaco = (c == ' ' || c == '\t' || c == '\n' || c == '\r');
            if (!ehEspaco) {
                ungetc(c, f);
                return;
            }
        }
        c = fgetc(f);
    }
}

Imagem *abrirImagem(const char *nomeImagem) {
    FILE *arquivo = fopen(nomeImagem, "rb"); // "rb" = read binary 
    if (!arquivo) 
    {
        fprintf(stderr, "Erro ao abrir o arquivo em abrirImagem.\n");
        return NULL;
    }

    char tipo[3];
    fscanf(arquivo, "%2s", tipo);
    if (strcmp(tipo, "P5") != 0 && strcmp(tipo, "P2") != 0) {
        fprintf(stderr, "Apenas formato P5 ou P2 suportados.\n");
        fclose(arquivo);
        return NULL;
    }

    Imagem *img = malloc(sizeof(Imagem));
    if (!img) 
    {
        fclose(arquivo);
        return NULL;
    }

    skipEspacosEComentarios(arquivo);

    fscanf(arquivo, "%d", &img->largura);

    skipEspacosEComentarios(arquivo);

    fscanf(arquivo, "%d", &img->altura);

    skipEspacosEComentarios(arquivo);

    fscanf(arquivo, "%d", &img->valorMax);



    // para P5 consome exatamente um byte de espaço antes dos dados binários 
    if (strcmp(tipo, "P5") == 0)
        fgetc(arquivo);

    img->pixels = malloc(img->altura * sizeof(int *));
    for (int i = 0; i < img->altura; i++)
        img->pixels[i] = malloc(img->largura * sizeof(int));

    // acima faz a alocação dinâmica da matriz de pixels, onde cada linha é um ponteiro para um array de inteiros
    //  representando os pixels daquela linha.
    

    // para P5, os dados dos pixels são lidos como bytes (ou dois bytes para valores maiores que 255)
    if (strcmp(tipo, "P5") == 0) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                if (img->valorMax < 256)
                    img->pixels[i][j] = fgetc(arquivo);
                else {
                    int msb = fgetc(arquivo);
                    int lsb = fgetc(arquivo);
                    img->pixels[i][j] = msb *  256 + lsb;
                }
            }
        }

    // enquanto para P2, os pixels são lidos como inteiros em formato de texto.
    } else /* P2 */
    {
        for (int i = 0; i < img->altura; i++)
            for (int j = 0; j < img->largura; j++)
                fscanf(arquivo, "%d", &img->pixels[i][j]);
    }

    fclose(arquivo);
    return img;
}

void liberarImagem(Imagem *imagem) {
    for (int i = 0; i < imagem->altura; i++) 
        free(imagem->pixels[i]);
        
    free(imagem->pixels);
    free(imagem);
}

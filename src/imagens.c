#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagens.h"
#include "fila.h"
#include "funcoes_aux.h"


/* avança o ponteiro do arquivo ignorando espaços em branco e linhas de comentário (#) */
static void skipEspacosEComentarios(FILE *f)
{
    int c;
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '#')
        {
            while ((c = fgetc(f)) != '\n' && c != EOF);
        }
        else if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            ungetc(c, f);
            return;
        }
    }
}

Imagem *abrirImagem(const char *nomeImagem)
{
    FILE *arquivo = fopen(nomeImagem, "rb");
    if (!arquivo)
    {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return NULL;
    }

    char tipo[3];
    fscanf(arquivo, "%2s", tipo);
    if (strcmp(tipo, "P5") != 0 && strcmp(tipo, "P2") != 0)
    {
        fprintf(stderr, "Apenas formato P5 ou P2 suportados.\n");
        fclose(arquivo);
        return NULL;
    }

    Imagem *img = malloc(sizeof(Imagem));
    if (!img) { fclose(arquivo); return NULL; }

    skipEspacosEComentarios(arquivo);
    fscanf(arquivo, "%d", &img->largura);
    skipEspacosEComentarios(arquivo);
    fscanf(arquivo, "%d", &img->altura);
    skipEspacosEComentarios(arquivo);
    fscanf(arquivo, "%d", &img->valorMax);

    /* para P5 consome exatamente um byte de espaço antes dos dados binários */
    if (strcmp(tipo, "P5") == 0)
        fgetc(arquivo);

    img->pixels = malloc(img->altura * sizeof(int *));
    for (int i = 0; i < img->altura; i++)
        img->pixels[i] = malloc(img->largura * sizeof(int));

    if (strcmp(tipo, "P5") == 0)
    {
        for (int i = 0; i < img->altura; i++)
        {
            for (int j = 0; j < img->largura; j++)
            {
                if (img->valorMax < 256)
                    img->pixels[i][j] = fgetc(arquivo);
                else
                {
                    int msb = fgetc(arquivo);
                    int lsb = fgetc(arquivo);
                    img->pixels[i][j] = (msb << 8) | lsb;
                }
            }
        }
    }
    else /* P2 */
    {
        for (int i = 0; i < img->altura; i++)
            for (int j = 0; j < img->largura; j++)
                fscanf(arquivo, "%d", &img->pixels[i][j]);
    }

    fclose(arquivo);
    return img;
}

void liberarImagem(Imagem *imagem)
{
    for (int i = 0; i < imagem->altura; i++)
        free(imagem->pixels[i]);
    free(imagem->pixels);
    free(imagem);
}

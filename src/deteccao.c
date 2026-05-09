#include <stdio.h>
#include "deteccao.h"   
#include "fila.h"
#include "funcoes_aux.h"


int floodFill(Imagem *img, int **rotulos, int linhaInicial, int colunaInicial, int id)
{
    int limiar = img->valorMax / 2;
    int area   = 0;

    // a fila precisa comportar no máximo todos os pixels da imagem
    Fila *fila = criarFila(img->altura * img->largura);
    enfileirar(fila, linhaInicial, colunaInicial);
    rotulos[linhaInicial][colunaInicial] = id;

    // direções: cima, baixo, esquerda, direita
    int direcaoLinha[]   = {-1, 1,  0, 0};
    int direcaoColuna[]  = { 0, 0, -1, 1};

    while(!filaVazia(fila))
    {
        int linha, coluna;
        desenfileirar(fila, &linha, &coluna);
        area++;

        // verifica os 4 vizinhos
        for(int d = 0; d < 4; d++)
        {
            int vizinhoLinha  = linha  + direcaoLinha[d];
            int vizinhoColuna = coluna + direcaoColuna[d];

            // checa se está dentro da imagem
            if(vizinhoLinha < 0 || vizinhoLinha >= img->altura)  continue;
            if(vizinhoColuna < 0 || vizinhoColuna >= img->largura) continue;

            // checa se já foi visitado
            if(rotulos[vizinhoLinha][vizinhoColuna] != 0) continue;

            // checa se é pixel de objeto (escuro)
            if(img->pixels[vizinhoLinha][vizinhoColuna] > limiar) continue;

            // marca e enfileira
            rotulos[vizinhoLinha][vizinhoColuna] = id;
            enfileirar(fila, vizinhoLinha, vizinhoColuna);
        }
    }

    liberarFila(fila);
    return area;
}

Deteccao *detectarObjetos(Imagem *img)
{
    Deteccao *det = malloc(sizeof(Deteccao));
    det->totalObjetos = 0;

    // aloca a matriz de rótulos (começa tudo zerado = fundo)
    det->rotulos = malloc(img->altura * sizeof(int *));
    for(int i = 0; i < img->altura; i++)
    {
        det->rotulos[i] = malloc(img->largura * sizeof(int));
        for(int j = 0; j < img->largura; j++)
            det->rotulos[i][j] = 0;
    }

    // aloca lista de objetos (no máximo um por pixel, na prática bem menos)
    det->objetos = malloc(img->altura * img->largura * sizeof(Objeto));

    int limiar = img->valorMax / 2;

    // varre a imagem de cima pra baixo, esquerda pra direita
    for(int i = 0; i < img->altura; i++)
    {
        for(int j = 0; j < img->largura; j++)
        {
            // pixel escuro e ainda não visitado = novo objeto
            if(img->pixels[i][j] <= limiar && det->rotulos[i][j] == 0)
            {
                int id = det->totalObjetos + 1;
                int area = floodFill(img, det->rotulos, i, j, id);

                det->objetos[det->totalObjetos].id   = id;
                det->objetos[det->totalObjetos].x    = j;  // coluna = x
                det->objetos[det->totalObjetos].y    = i;  // linha  = y
                det->objetos[det->totalObjetos].area = area;
                det->totalObjetos++;
            }
        }
    }

    return det;
}

void liberarDeteccao(Deteccao *det, int altura)
{
    for(int i = 0; i < altura; i++)
        free(det->rotulos[i]);
    free(det->rotulos);
    free(det->objetos);
    free(det);
}

static int cores[][3] = {
    {255,   0,   0},   // vermelho
    {  0, 255,   0},   // verde
    {  0,   0, 255},   // azul
    {255, 255,   0},   // amarelo
    {255,   0, 255},   // magenta
    {  0, 255, 255},   // ciano
    {255, 128,   0},   // laranja
    {128,   0, 255},   // roxo
};
int totalCores = 8;

void gerarImagemSaida(Imagem *img, Deteccao *det, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if(!arquivo)
    {
        fprintf(stderr, "Erro ao criar arquivo de saida.\n");
        return;
    }

    // cabeçalho PPM
    fprintf(arquivo, "P6\n%d %d\n255\n", img->largura, img->altura);

    // percorre a matriz de rótulos e escreve as cores
    for(int i = 0; i < img->altura; i++)
    {
        for(int j = 0; j < img->largura; j++)
        {
            int rotulo = det->rotulos[i][j];

            if(rotulo == 0)
            {
                // fundo pintado de branco
                unsigned char branco[3] = {255, 255, 255};
                fwrite(branco, 1, 3, arquivo);
            }
            else
            {
                // escolhe a cor do objeto com base no seu índice. 
                int indiceCor = (rotulo - 1) % totalCores;
                unsigned char cor[3] = {
                    cores[indiceCor][0],
                    cores[indiceCor][1],
                    cores[indiceCor][2]
                };
                fwrite(cor, 1, 3, arquivo);
            }
        }
    }

    fclose(arquivo);
}
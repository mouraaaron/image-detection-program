#include "deteccao.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fila.h"
#include "funcoes_aux.h"

int floodFill(Imagem *img, int **rotulos, int linhaInicial, int colunaInicial, int id,
              int *minLinha, int *maxLinha, int *minColuna, int *maxColuna) {
    int limiar = img->valorMax / 2;
    int area = 0;

    *minLinha = linhaInicial;
    *maxLinha = linhaInicial;
    *minColuna = colunaInicial;
    *maxColuna = colunaInicial;

    Fila *fila = criarFila(img->altura * img->largura);
    enfileirar(fila, linhaInicial, colunaInicial);
    rotulos[linhaInicial][colunaInicial] = id;

    int direcaoLinha[] = {-1, 1, 0, 0};
    int direcaoColuna[] = {0, 0, -1, 1};
    // o trecho acima define os deslocamentos para as quatro direções (cima, baixo, esquerda, direita) 

    while (!filaVazia(fila)) {
        int linha, coluna;
        desenfileirar(fila, &linha, &coluna);
        area++;

        if (linha < *minLinha) *minLinha = linha;
        if (linha > *maxLinha) *maxLinha = linha;
        if (coluna < *minColuna) *minColuna = coluna;
        if (coluna > *maxColuna) *maxColuna = coluna;

        for (int d = 0; d < 4; d++) {
            int vl = linha + direcaoLinha[d];
            int vc = coluna + direcaoColuna[d];

            if(vl >= 0 && vl < img->altura && vc >= 0 && vc < img->largura && 
                img->pixels[vl][vc] <= limiar && rotulos[vl][vc] == 0)
            {

                rotulos[vl][vc] = id;
                enfileirar(fila, vl, vc);
            }
        }
    }

    liberarFila(fila);
    return area;
}

int **criarMascara(int **rotulos, int id, int minL, int maxL, int minC, int maxC) {
    //cria uma matriz com apenas o objeto detectado 
    int h = maxL - minL + 1;

    int w = maxC - minC + 1;

    int **m = malloc(h * sizeof(int *));

    for (int r = 0; r < h; r++)
    {
        m[r] = malloc(w * sizeof(int));

        for (int c = 0; c < w; c++)
        {
            if(rotulos[minL + r][minC + c] == id)
                m[r][c] = 1;
            else
                m[r][c] = 0;
        }
    }
    return m;
}

void liberarMascara(int **mascara, int altura) {
    for (int i = 0; i < altura; i++)
        free(mascara[i]);
    free(mascara);
}

int objetosIguais(Objeto *a, Objeto *b) {
    if (a->larguraBBox != b->larguraBBox) return 0;
    if (a->alturaBBox != b->alturaBBox) return 0;

    for (int r = 0; r < a->alturaBBox; r++)
        for (int c = 0; c < a->larguraBBox; c++)
            if (a->mascara[r][c] != b->mascara[r][c]) 
                return 0;

    return 1;
    //retorna 1 se os objetos forem iguais (mesma forma), ou 0 caso contrário.
}

Deteccao *detectarObjetos(Imagem *img) {
    Deteccao *det = malloc(sizeof(Deteccao));
    det->totalObjetos = 0;

    det->rotulos = malloc(img->altura * sizeof(int *));
    for (int i = 0; i < img->altura; i++) {
        det->rotulos[i] = malloc(img->largura * sizeof(int));

        for (int j = 0; j < img->largura; j++) 
            det->rotulos[i][j] = 0;
    }
    /* o trecho acima cria uma matriz chamada rótulos que tem as mesmas dimensões da imagem de entrada. 
    Cada elemento dessa matriz é inicializado com o valor 0, indicando que nenhum pixel foi rotulado ainda. */
    

    det->objetos = malloc(img->altura * img->largura * sizeof(Objeto));

    int limiar = img->valorMax / 2;

    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            if (img->pixels[i][j] <= limiar && det->rotulos[i][j] == 0) { 
                int id = det->totalObjetos + 1;
                int minL, maxL, minC, maxC;
                int area = floodFill(img, det->rotulos, i, j, id, &minL, &maxL, &minC, &maxC);

                Objeto *obj = &det->objetos[det->totalObjetos];
                obj->id = id;
                obj->x = j;
                obj->y = i;
                obj->area = (float)area;
                obj->minLinha = minL;
                obj->maxLinha = maxL;
                obj->minColuna = minC;
                obj->maxColuna = maxC;
                obj->alturaBBox = maxL - minL + 1;
                obj->larguraBBox = maxC - minC + 1;
                obj->mascara = criarMascara(det->rotulos, id, minL, maxL, minC, maxC);
                // para cada objeto, cria-se uma máscara binária que representa a forma do objeto dentro de seu bounding box.]
                // a máscara é uma matriz contendo APENAS 1 objeto

                det->totalObjetos++;
            }
        }
    }

    // atribui índice de cor. objetos com mesma forma recebem o mesmo índice 
    det->corDoObjeto = malloc(det->totalObjetos * sizeof(int));

    for (int i = 0; i < det->totalObjetos; i++) 
        det->corDoObjeto[i] = -1;

    int proximaCor = 0;

    for (int i = 0; i < det->totalObjetos; i++) {

        if (det->corDoObjeto[i] != -1){
            det->corDoObjeto[i] = proximaCor;
            for (int j = i + 1; j < det->totalObjetos; j++) {
                if (det->corDoObjeto[j] == -1 && (objetosIguais(&det->objetos[i], &det->objetos[j]) != 0))
                    det->corDoObjeto[j] = proximaCor;
                }
        }
        //dá uma cor para um objeto. procura por objetos iguais; dá a mesma cor para eles caso forem iguais
        proximaCor++;
    }

    return det;
}

void liberarDeteccao(Deteccao *det, int altura) {
    for (int i = 0; i < altura; i++) free(det->rotulos[i]);
    free(det->rotulos);

    for (int i = 0; i < det->totalObjetos; i++)
        liberarMascara(det->objetos[i].mascara, det->objetos[i].alturaBBox);

    free(det->objetos);
    free(det->corDoObjeto);
    free(det);
}

static int cores[][3] = {
    {220, 20, 20},  // vermelho escuro
    {0, 180, 0},    // verde
    {0, 0, 220},    // azul
    {220, 220, 0},  // amarelo
    {220, 0, 220},  // magenta
    {0, 200, 200},  // ciano
    {255, 128, 0},  // laranja
    {128, 0, 255},  // roxo
};
static int totalCores = 8;

void gerarImagemSaida(Imagem *img, Deteccao *det, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        fprintf(stderr, "Erro ao criar arquivo de saida.\n");
        return;
    }

    fprintf(arquivo, "P6\n%d %d\n255\n", img->largura, img->altura);

    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            int rotulo = det->rotulos[i][j];
            if (rotulo == 0) {
                unsigned char branco[3] = {255, 255, 255};
                fwrite(branco, 1, 3, arquivo);
            } else {
                int indiceCor = det->corDoObjeto[rotulo - 1] % totalCores;
                unsigned char cor[3] = {(unsigned char)cores[indiceCor][0],
                                        (unsigned char)cores[indiceCor][1],
                                        (unsigned char)cores[indiceCor][2]};
                fwrite(cor, 1, 3, arquivo);
            }
        }
    }

    fclose(arquivo);
}

int comparadorArea(const void *a, const void *b) {
    const Objeto *oa = (const Objeto *)a;
    const Objeto *ob = (const Objeto *)b;
    if (ob->area > oa->area) return 1;
    if (ob->area < oa->area) return -1;
    return 0;
}

void imprimirListaOrdenada(Deteccao *det) {
    Objeto *sorted = malloc(det->totalObjetos * sizeof(Objeto));
    memcpy(sorted, det->objetos, det->totalObjetos * sizeof(Objeto));
    qsort(sorted, det->totalObjetos, sizeof(Objeto), comparadorArea);

    printf("Total de objetos encontrados: %d\n", det->totalObjetos);
    for (int i = 0; i < det->totalObjetos; i++) {
        printf("Objeto %d (Posição x=%d, y=%d): Área = %.1f pixels\n", sorted[i].id, sorted[i].x,
               sorted[i].y, sorted[i].area);
    }

    free(sorted);
}

#define MARGEM 10

void gerarImagemMaioresObjetos(Imagem *img, Deteccao *det, const char *nomeArquivo) {
    (void)img;
    if (det->totalObjetos == 0) return;

    Objeto *sorted = malloc(det->totalObjetos * sizeof(Objeto));
    memcpy(sorted, det->objetos, det->totalObjetos * sizeof(Objeto));
    qsort(sorted, det->totalObjetos, sizeof(Objeto), comparadorArea);

    int n = det->totalObjetos < 3 ? det->totalObjetos : 3;

    /* largura total: margem esquerda + (largura + margem) de cada objeto */
    int totalLargura = MARGEM;
    int maxAltura = 0;
    for (int k = 0; k < n; k++) {
        totalLargura += sorted[k].larguraBBox + MARGEM;
        if (sorted[k].alturaBBox > maxAltura) maxAltura = sorted[k].alturaBBox;
    }
    int totalAltura = maxAltura + 2 * MARGEM;

    /* calcula o deslocamento X de cada objeto no canvas */
    int *xOffset = malloc(n * sizeof(int));
    xOffset[0] = MARGEM;
    for (int k = 1; k < n; k++) xOffset[k] = xOffset[k - 1] + sorted[k - 1].larguraBBox + MARGEM;

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        fprintf(stderr, "Erro ao criar arquivo de maiores objetos.\n");
        free(xOffset);
        free(sorted);
        return;
    }

    fprintf(arquivo, "P6\n%d %d\n255\n", totalLargura, totalAltura);

    for (int row = 0; row < totalAltura; row++) {
        for (int col = 0; col < totalLargura; col++) {
            unsigned char pixel[3] = {255, 255, 255}; /* branco por padrão */

            for (int k = 0; k < n; k++) {
                int objCol = col - xOffset[k];
                int objRow = row - MARGEM;

                if (objCol >= 0 && objCol < sorted[k].larguraBBox && objRow >= 0 &&
                    objRow < sorted[k].alturaBBox && sorted[k].mascara[objRow][objCol]) {
                    pixel[0] = pixel[1] = pixel[2] = 0; /* preto */
                    break;
                }
            }

            fwrite(pixel, 1, 3, arquivo);
        }
    }

    fclose(arquivo);
    free(xOffset);
    free(sorted);
}

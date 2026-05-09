#include "funcoes_aux.h"
#include "imagens.h"
#include "deteccao.h"
#include "fila.h"  
#include <stdio.h>





int main(int argc, char *argv[]) 
{

    parametros parametros;

    if (coletarParametros(argc, argv, &parametros) != 0)
    {
        fprintf(stderr, "Erro ao coletar parâmetros.\n");
        return 1;
    }

    Imagem *imagem = abrirImagem(parametros.primArq[0]);
    if(imagem == NULL)
    {
        fprintf(stderr, "Erro ao abrir a imagem.\n");
        return 1;
    }

    Deteccao *det = detectarObjetos(imagem);
    gerarImagemSaida(imagem, det, parametros.segArq[0]);

    printf("Total de objetos detectados: %d\n", det->totalObjetos);
    for(int i = 0; i < det->totalObjetos; i++)
    {
        Objeto obj = det->objetos[i];
        printf("Objeto %d: Area = %d, Posição = (%d, %d)\n", obj.id, obj.area, obj.x, obj.y);
    }

    liberarDeteccao(det, imagem->altura);

    liberarImagem(imagem);

    return 0;
}
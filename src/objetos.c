#include <stdio.h>

#include "deteccao.h"
#include "fila.h"
#include "funcoes_aux.h"
#include "imagens.h"

int main(int argc, char *argv[]) {
    parametros params;
    params.primArq[0] = NULL;
    params.segArq[0] = NULL;
    params.terArq[0] = NULL;

    // use a mesma estratégia de coletar parâmetros do trabalho #1
    if (coletarParametros(argc, argv, &params) != 0) {
        fprintf(stderr, "Erro ao coletar parametros.\n");
        return 1;
    }

    if (params.primArq[0] == NULL || params.segArq[0] == NULL) {
        fprintf(stderr, "Erro na linha de comando");
        return 1;
    }

    Imagem *imagem = abrirImagem(params.primArq[0]);
    
    if (imagem == NULL) 
    {
        fprintf(stderr, "Erro ao abrir a imagem.\n");
        return 1;
    }

    Deteccao *det = detectarObjetos(imagem);

    gerarImagemSaida(imagem, det, params.segArq[0]);

    imprimirListaOrdenada(det);

    if (params.terArq[0] != NULL) 
        gerarImagemMaioresObjetos(det, params.terArq[0]);

    liberarDeteccao(det, imagem->altura);
    liberarImagem(imagem);

    return 0;
}

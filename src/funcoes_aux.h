#ifndef FUNCOES_H
#define FUNCOES_H
#include <stdio.h>
#include <stdlib.h>

#include "imagens.h"

typedef struct {
    char *primArq[15];
    char *segArq[15];
    char *terArq[15];

} parametros;

int coletarParametros(int argc, char *argv[], parametros *parametros);

#endif
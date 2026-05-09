#include <stdio.h>
#include <string.h>
#include "funcoes_aux.h"
#include "fila.h"




// usei o mesmo método do Trabalho1 de criar um struct para os parâmetros. 
int coletarParametros(int argc, char *argv[], parametros *parametros)
{
    for(int i = 1; i < argc;)
    {
        if(strcmp(argv[i] , "-i") == 0 )
        {
            parametros->primArq[0] = argv[i+1];
            i += 2;
        }
        else if(strcmp(argv[i] , "-o") == 0 )
        {
            parametros->segArq[0] = argv[i+1];
            i += 2;
        }
        else if(strcmp(argv[i] , "-a") == 0 )
        {
            parametros->terArq[0] = argv[i+1];
            i += 2;
        }
        else
        {
            break;
        }
    }
    return 0;
}   




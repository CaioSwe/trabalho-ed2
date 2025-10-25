#ifndef _QRY_h_
#define _QRY_h_

#include "lista.h"
#include "streap.h"
#include "digraph.h"

/*
    MUDAR ALGUMAS COISINHAS BOBAS
*/

/**
 * @brief Processa o arquivo .qry passado como parâmetro e insere as formas adquiridas em uma lista de retorno com formas do tipo Quadra.
 * 
 * @param path String de caminho do arquivo .qry.
 * @return Retorna uma lista com as formas do .qry.
 */
Lista processQryFile(Graph grafo, STreap arvore, const char* path);

#endif
#ifndef _QRY_h_
#define _QRY_h_

#include "lista.h"
#include "streap.h"
#include "digraph.h"
#include "geo.h"

/*
    TERMINAR A IMPLEMENTACAO DA STREAP PARA TESTE FINAL.
    TERMINAR IMPLEMENTACAO DO removerQuadras E desbloquearSentido.
*/

typedef void* Percurso;

/**
 * @brief Processa o arquivo .qry.
 * 
 * @param grafo Grafo da via.
 * @param quadras Quadras da cidade.
 * @param path String de caminho do arquivo .qry.
 * @return O caminho da origem ate' o destino do .qry. Caso o caminho nao exista, retorna nulo.
 */
Percurso processQryFile(Graph grafo, Quadras quadras, const char* path);

// FUNCOES GET

Caminho getCaminhoCurto(Percurso percurso);

Caminho getCaminhoRapido(Percurso percurso);

const char* getPercursoRapidoColor(Percurso percurso);

const char* getPercursoCurtoColor(Percurso percurso);

#endif
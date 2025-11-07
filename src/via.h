#ifndef _VIA_h_
#define _VIA_h_

#include "digraph.h"

typedef void* VerticeVia;
typedef void* ArestaVia;

/**
 * @brief Processa o arquivo .via passado como parâmetro e insere as informacoes adquiridas em um grafo de retorno.
 * 
 * @param path String de caminho do arquivo .via.
 * @return Retorna um grafo direcionado com as informacoes lidas no arquivo .via.
*/
Graph processViaFile(const char* path);

// FUNCOES SET

// Desabilita a aresta.
void blockAresta(ArestaVia av);

// Habilita a aresta.
void unblockAresta(ArestaVia av);

// FUNCOES GET

// Retorna o ponto X do ve'rtice.
double getVerticeViaX(VerticeVia vv);

// Retorna o ponto Y do ve'rtice.
double getVerticeViaY(VerticeVia vv);

// Retorna o valor booleano do estado da aresta.
bool isArestaEnabled(ArestaVia av);


#endif
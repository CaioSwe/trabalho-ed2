#ifndef _VIA_h_
#define _VIA_h_

#include "digraph.h"

/**
 * Cabeçalho dedicado ao processamento do arquivo .via, que tem como elementos:
 * [v] Vértice de âncora (x, y) e um nome.
 * [e] Aresta (u, v) tal que u e v são vértices, velocidade média (vm), comprimento (cmp), quadra à direita e esquerda, nome
 * 
 * O arquivo .via tem como objetivo montar um grafo direcionado com os elementos lidos, com arestas de peso vm e cmp.
 */

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
void blockVia(ArestaVia av);

// Habilita a aresta.
void unblockVia(ArestaVia av);

// Muda a velocidade me'dia da aresta.
void setArestaVM(ArestaVia av, double vm);

// FUNCOES GET

// Retorna o ponto X do ve'rtice.
double getVerticeViaX(VerticeVia vv);

// Retorna o ponto Y do ve'rtice.
double getVerticeViaY(VerticeVia vv);

// Retorna o nome da aresta.
const char* getArestaViaNome(ArestaVia av);

// Retorna a velocidade me'dia da aresta.
double getArestaVM(ArestaVia av);

// Retorna o comprimento da aresta.
double getArestaCMP(ArestaVia av);

// Retorna o valor booleano do estado da aresta.
bool isArestaEnabled(ArestaVia av);

// FREE

// Libera a memoria alocada para as vias de arestas.
void freeArestaVia(ArestaVia av, void* extra);

#endif
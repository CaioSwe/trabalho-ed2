#ifndef _FILES_h_
#define _FILES_h_

#include "geo.h"
#include "lista.h"
#include "digraph.h"

/**
 * Cabeçalho dedicado às funções relacionadas a arquivos.
 * Mais especificamente às funções que usam estruturas internas do programa,
 * como Grafo e Quadras.
*/

/**
 * @brief Coloca a quadra especificada no arquivo file.
 * 
 * @param quadra Ponteiro de uma estrutura de Quadra.
 * @param file Ponteiro de um arquivo SVG aberto em modo de escrita.
 * @return Não há retorno de algum valor.
 */
void printQuadraToSVG(Quadra quadra, FILE* file);

/**
 * @brief Coloca um Ve'rtice do grafo g especifico no arquivo file.
 * 
 * @param g Grafo g.
 * @param node Nu'mero do ve'rtice assocido ao grafo g.
 * @param file Ponteiro de um arquivo SVG aberto em modo de escrita.
 * @param color String de cor a ser pintada o ve'rtice.
 * @return Não há retorno de algum valor.
 */
void printVerticeToSVG(Graph g, Node node, FILE* file, const char* color);

/**
 * @brief Coloca uma Aresta do grafo G especifico no arquivo file.
 * 
 * @param g Grafo g.
 * @param e Ponteiro para uma aresta associada ao grafo g.
 * @param file Ponteiro de um arquivo SVG aberto em modo de escrita.
 * @param color String de cor a ser pintada a aresta.
 * @return Não há retorno de algum valor.
 */
void printEdgeToSVG(Graph g, Edge e, FILE* file, const char* color);

#endif
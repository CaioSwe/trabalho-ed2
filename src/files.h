#ifndef _FILES_h_
#define _FILES_h_

#include "geo.h"
#include "lista.h"

#include "digraph.h"

typedef struct teste1{
    FILE* file;
    Graph grafo;
    int cor;
}teste1;

/**
 * @brief Coloca a quadra especificada no arquivo file.
 * 
 * @param quadra Ponteiro de uma estrutura de Quadra.
 * @param x Posicao horizontal da quadra.
 * @param y Posicao vertical da quadra.
 * @param mbbX1 Posicao horizontal 1 do bounding box da quadra.
 * @param mbbY1 Posicao vertical 1 do bounding box da quadra.
 * @param mbbX2 Posicao horizontal 2 do bounding box da quadra.
 * @param mbbY2 Posicao vertical 2 do bounding box da quadra.
 * @param file Ponteiro de um arquivo aberto no modo de escrita.
 * @return Não há retorno de algum valor.
 */
void printQuadrasToSVG(Quadra quadra, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *file);

/**
 * @brief Coloca um item (Ve'rtice) especifico no arquivo file.
 * 
 * @param item Ponteiro de uma estrutura / valor.
 * @param file Ponteiro de um arquivo aberto no modo de escrita.
 * @return Não há retorno de algum valor.
 */
void printVerticesToSVG(Item item, void* file);

/**
 * @brief Coloca um item (Aresta) especifico no arquivo file.
 * 
 * @param item Ponteiro de uma estrutura / valor.
 * @param file Ponteiro de um arquivo aberto no modo de escrita.
 * @return Não há retorno de algum valor.
 */
void printEdgesToSVG(Item item, void* extra);

// ADAPT: void printToTXT(const void* item, const void* file, const void* aux);

// void printToTXT(Item item, void* extra);

#endif
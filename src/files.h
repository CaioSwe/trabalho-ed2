#ifndef _FILES_h_
#define _FILES_h_

#include "geo.h"
#include "lista.h"

/**
 * @brief Coloca a quadra especificada no arquivo file.
 * 
 * @param quadra Ponteiro de uma estrutura de Quadra.
 * @param file Ponteiro de um arquivo aberto no modo de escrita.
 * @return Não há retorno de algum valor.
 */
void printQuadrasToSVG(Quadra quadra, void* file);

/**
 * @brief Coloca o item especifico no arquivo file.
 * 
 * @param item Ponteiro de uma estrutura / valor.
 * @param file Ponteiro de um arquivo aberto no modo de escrita.
 * @return Não há retorno de algum valor.
 */
void printVerticesToSVG(Item item, void* file);

// ADAPT: void printToTXT(const void* item, const void* file, const void* aux);

#endif
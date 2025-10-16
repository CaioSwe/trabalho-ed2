#ifndef _GEO_h_
#define _GEO_h_

#include "lista.h"

typedef void* Quadra;

/**
 * @brief Processa o arquivo .geo passado como parâmetro e insere as formas adquiridas em uma lista de retorno com formas do tipo Quadra.
 * 
 * @param path String de caminho do arquivo .geo.
 * @return Retorna uma lista com as formas do .geo.
 */
Lista processGeoFile(const char* path);

// FUNÇÕES GET

// Retorna o ponto X da quadra.
double getQuadraX(const void* quadra);

// Retorna o ponto Y da quadra.
double getQuadraY(const void* quadra);

// Retorna o valor de largura da quadra.
double getQuadraWidth(const void* quadra);

// Retorna o valor de altura da quadra.
double getQuadraHeight(const void* quadra);

// Retorna a cor de preenchimento da quadra.
const char* getQuadraCFill(const void* quadra);

// Retorna a cor de borda da quadra.
const char* getQuadraCStrk(const void* quadra);

// Retorna a espessura da borda da quadra.
const char* getQuadraSW(const void* quadra);

#endif
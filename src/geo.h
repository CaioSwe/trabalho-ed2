#ifndef _GEO_h_
#define _GEO_h_

#include "lista.h"
#include "hash.h"
#include "streap.h"

/**
 * Cabeçalho dedicado à leitura do arquivo .geo,
 * suas formas geométricas lidas no mesmo,
 * bem como suas funções de get e set.
 * 
 * Cada estrutura de uma forma geométrica contém uma estrutura que guarda
 * um array das formas, quantidade das formas lidas e uma Tabela Hash para
 * associação de Nome - ID.
 * 
 * Formas nas quais se encontram nesse cabeçalho: Quadra [retângulo].
*/

typedef void* Quadra;
typedef void* Quadras;

/**
 * @brief Processa o arquivo .geo passado como parâmetro e insere as formas adquiridas em uma lista de retorno com formas do tipo Quadra.
 * 
 * @param path String de caminho do arquivo .geo.
 * @return Retorna uma lista com as formas do .geo.
*/
Quadras processGeoFile(const char* path);

/**
 * @brief Percorre as quadras e passa cada quadra para a função F.
 * @param quadras 'Quadras' associada aos valores a serem percorridos.
 * @param runFunc Ponteiro para uma função externa que têm como paramêtro uma Quadra e um ponteiro para uma variável auxiliar.
 * @param extra Ponteiro para uma variável qualquer auxiliar.
 * @return Não há retorno de algum valor.
*/
void percorrerQuadras(Quadras quadras, FvisitaNo runFunc, void* extra);

// FUNÇÕES GET

// Retorna o ID da quadra.
const char* getQuadraID(Quadra quadra);

// Retorna o ponto X da quadra.
double getQuadraX(Quadra quadra);

// Retorna o ponto Y da quadra.
double getQuadraY(Quadra quadra);

// Retorna o valor de largura da quadra.
double getQuadraWidth(Quadra quadra);

// Retorna o valor de altura da quadra.
double getQuadraHeight(Quadra quadra);

// Retorna a cor de preenchimento da quadra.
const char* getQuadraCFill(Quadra quadra);

// Retorna a cor de borda da quadra.
const char* getQuadraCStrk(Quadra quadra);

// Retorna a espessura da borda da quadra.
const char* getQuadraSW(Quadra quadra);

// Retorna a quadra associada ao id fornecido.
Quadra getQuadraByID(Quadras quadras, const char* id);

// Insere as quadras que estao dentro da regiao na lista passada.
void getQuadrasRegion(Quadras quadras, double x, double y, double w, double h, Lista resultado);

// FUNÇÕES SET

// Define a cor de preenchimento da quadra.
void setQuadraCFill(Quadra quadra, const char* cfill);

// Define a cor de borda da quadra.
void setQuadraCStrk(Quadra quadra, const char* cstrk);

// Remove a quadra especifica da estrutura Quadras.
void removerQuadra(Quadras quadras, Quadra quadra);

// FUNCOES FREE

// Funcao de liberacao de memoria das quadras.
void freeQuadras(Quadras quadras, void* extra);

/////////////////////////////////

STreap getQuadrasSTrp(Quadras quadras);

#endif
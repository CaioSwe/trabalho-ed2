#ifndef _QRY_h_
#define _QRY_h_

#include "digraph.h"
#include "geo.h"

/**
 * Cabeçalho de processamento do arquivo .qry com operações:
 * [@o?] : Atribui um ponto de origem para o percurso.
 * [catac] : Destroi quadras e arestas incidentes na região.
 * [b] : Aumenta a velocidade média das arestas.
 * [blq] : Bloqueia um determinado sentido de uma região de arestas.
 * [rbl] : Remove o bloqueio que foi previamente ativado.
 * [@p?] : Atribui um ponto de destino para o percurso.
 * 
 * O arquivo .qry tem como objetivo utilizar o algoritmo de Dijkstra para achar
 * caminho mais rápido e o mais curto entre dois pontos do grafo passado.
 */

typedef void* Percurso;

/**
 * @brief Processa o arquivo .qry e faz um arquivo texto (TXT) com as modificacoes.
 * 
 * @param grafo Grafo da via.
 * @param quadras Quadras da cidade.
 * @param path String de caminho do arquivo .qry.
 * @param svg Arquivo .svg aberto em modo de escrita.
 * @param txtPath String de caminho para output das modificacoes em .txt.
 * @return Nao ha' retorno de algum valor.
 */
void processQryFile(Graph grafo, Quadras quadras, const char* path, FILE* svg, const char* txtPath);

#endif
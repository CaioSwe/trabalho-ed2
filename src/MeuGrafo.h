#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <raylib.h>
#include <raymath.h>
#include "MinhaLista.h"

typedef struct Vertice {
    int id;
    Rectangle pos;
    Color cor;
    Lista* listaDestinos;
} Vertice;

typedef struct {
    Vertice* origem;
    Vertice* vertice;
    float value;
    Color cor;
} VerticeValue;

typedef struct {
    int numVertices;
    int numArestas;
    Lista* listaVertices;
} Grafo;

#define vector2Equals(a, b) (fabsf((a).x - (b).x) < (0.00001f) && fabsf((a).y - (b).y) < (0.00001f))

Grafo* criarGrafo();
Vertice* criarVertice(Grafo* grafo, Rectangle pos, Color cor);
void adicionarAresta(Grafo* grafo, Vertice* origem, Vertice* destino, float value);
void imprimirGrafo(Grafo* grafo);
void liberarGrafo(Grafo* grafo);

Vertice* getVerticeByID(Grafo* grafo, int id);

VerticeValue* getVerticeValueOf(Grafo* grafo, Vertice* origem, Vertice* destino);

void imprimirGrafoRay(Grafo* grafo);

void imprimirVerticeRay(Vertice* vertice);
void imprimirArestaRay(VerticeValue* vv);

#endif

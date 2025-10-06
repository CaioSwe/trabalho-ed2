#include <stdio.h>
#include <stdlib.h>
#include "MeuGrafo.h"

void DrawArrow(Vector2 start, Vector2 end, float headLength, float headWidth, float backOffset, Color color) {
    // Draw the shaft
    

    // Direction and normalized vector
    Vector2 direction = Vector2Subtract(end, start);
    float length = Vector2Length(direction);
    if (length < 1.0f) return;

    Vector2 normalizedDirection = Vector2Scale(direction, 1.0f / length);

    Vector2 a = Vector2Subtract(end, Vector2Scale(normalizedDirection, backOffset));
    Vector2 b = Vector2Add(start, Vector2Scale(normalizedDirection, backOffset));

    DrawLineEx(b, a, 2.0f, color);

    // Perpendicular for arrowhead
    Vector2 perpendicular = {-normalizedDirection.y, normalizedDirection.x};

    Vector2 basePoint = Vector2Subtract(a, Vector2Scale(normalizedDirection, headLength));

    Vector2 pointA = Vector2Add(basePoint, Vector2Scale(perpendicular, headWidth));
    Vector2 pointB = Vector2Subtract(basePoint, Vector2Scale(perpendicular, headWidth));
    Vector2 pointC = a;

    // Determine tip, left, right
    Vector2 tip, left, right;

    // 1️⃣ Tip is lowest Y
    if (pointC.y <= pointA.y && pointC.y <= pointB.y) tip = pointC;
    else if (pointA.y <= pointC.y && pointA.y <= pointB.y) tip = pointA;
    else tip = pointB;

    // 2️⃣ Right is highest X among remaining points
    if ((tip.x != pointC.x && pointC.x > pointA.x && pointC.x > pointB.x)) right = pointC;
    else if ((tip.x != pointA.x && pointA.x > pointC.x && pointA.x > pointB.x)) right = pointA;
    else right = pointB;

    // 3️⃣ Left is the remaining point
    if ((tip.x != pointC.x && right.x != pointC.x)) left = pointC;
    else if ((tip.x != pointA.x && right.x != pointA.x)) left = pointA;
    else left = pointB;

    if (vector2Equals(tip, right)){
        right = left;
        left = pointA;
    }

    if (vector2Equals(tip, left)){
        left = pointA;
    }

    if(right.x < left.x){
        Vector2 temp = left;
        left = right;
        right = temp;
    }

    DrawLineEx(pointC, pointA, 2.0f, color);
    DrawLineEx(pointC, pointB, 2.0f, color);

    // Draw the arrowhead triangle
    //DrawTriangle(tip, left, right, color);

    // // Draw labels for debugging
    // DrawText("Tip", tip.x - MeasureText("Tip", 10) / 2, tip.y - 10, 10, BLACK);
    // DrawText("Left", left.x - MeasureText("Left", 10) / 2, left.y - 10, 10, BLACK);
    // DrawText("Right", right.x - MeasureText("Right", 10) / 2, right.y - 10, 10, BLACK);
}

float id = 0;

// Cria um grafo com n vértices
Grafo* criarGrafo() {
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    if (!grafo) return NULL;

    grafo->numVertices = 0;
    grafo->numArestas = 0;

    grafo->listaVertices = criaLista();
    if (!grafo->listaVertices) {
        free(grafo);
        return NULL;
    }

    return grafo;
}

// Cria um novo nó
Vertice* criarVertice(Grafo* grafo, Rectangle pos, Color cor) {
    Vertice* novoVertice = (Vertice*)malloc(sizeof(Vertice));
    if (!novoVertice) return NULL;

    novoVertice->id = id++;
    novoVertice->pos = pos;
    novoVertice->cor = cor;

    Lista* novaListaDestinos = criaLista();

    novoVertice->listaDestinos = novaListaDestinos;

    inserirFim(grafo->listaVertices, novoVertice);
    grafo->numVertices++;

    return novoVertice;
}

static bool compararVertices(const void* item, const void* item2){
    return (((VerticeValue*)item)->vertice == item2);
}

static bool compararV(const void* item, const void* item2){
    return (((Vertice*)item) == ((Vertice*)item2));
}

// Adiciona uma aresta de origem -> destino
void adicionarAresta(Grafo* grafo, Vertice* origem, Vertice* destino, float value) {   
    if (!grafo || !origem || !destino || isInLista(origem->listaDestinos, compararVertices, destino)) return;

    VerticeValue* VV = (VerticeValue*)malloc(sizeof(VerticeValue));
    VV->value = value;
    VV->vertice = destino;
    VV->cor = WHITE;
    VV->origem = origem;

    inserirFim(origem->listaDestinos, VV);
    grafo->numArestas++;
}

static void imprimirGrafoDestinos(const void* g){
    VerticeValue* verticeV = (VerticeValue*)g;

    printf("(%.2f) %d ", verticeV->value, verticeV->vertice->id);
}

static void imprimirGrafoLista(const void* g){
    Vertice* lGrafo = (Vertice*)g;

    printf("ID: %d -> ", lGrafo->id);
    percorrerLista(lGrafo->listaDestinos, imprimirGrafoDestinos);
    printf("NULL\n");
}

// Imprime o grafo
void imprimirGrafo(Grafo* grafo) {
    if (!grafo) return;

    printf("Grafo com %d vertices e %d arestas:\n", grafo->numVertices, grafo->numArestas);

    percorrerLista(grafo->listaVertices, imprimirGrafoLista);
}

static void imprimirLinhasGrafo(const void* item, const void* aux){
    VerticeValue* destino = (VerticeValue*)item;
    Vertice* origem = (Vertice*)aux;

    if(origem == destino->vertice) DrawText("(self)", origem->pos.x - MeasureText("(self)", 10) / 2, origem->pos.y - 10.0f - 12, 10, WHITE);
    else{
        imprimirArestaRay(destino);
    }
}

static void imprimirListaGrafo(const void* lG){
    Vertice* listaGrafo = (Vertice*)lG;

    imprimirListaRel(listaGrafo->listaDestinos, listaGrafo, imprimirLinhasGrafo);
    imprimirVerticeRay(listaGrafo);
}

void imprimirGrafoRay(Grafo* grafo){
    imprimirLista(grafo->listaVertices, imprimirListaGrafo);
}

static void limparGrafoLista(const void* g){
    Vertice* listaGrafo = (Vertice*)g;

    limparLista(listaGrafo->listaDestinos, false);

    free(listaGrafo->listaDestinos);
    free(listaGrafo);
}

void liberarGrafo(Grafo* grafo) {
    if (!grafo) return;

    limparListaRel(grafo->listaVertices, limparGrafoLista, true);

    free(grafo);
}

Vertice* gotVertice = NULL;

void compareID(const void* item, const void* comp){
    if(((Vertice*)item)->id == *(int*)comp){
        gotVertice = (Vertice*)item;
        return;
    }
}

Vertice* getVerticeByID(Grafo* grafo, int id){
    gotVertice = NULL;
    int newid = id;
    percorrerListaRel(grafo->listaVertices, compareID, &newid);

    return gotVertice;
}

VerticeValue* vv = NULL;

void getVerticeValue(const void* item, const void* aux){
    if(((VerticeValue*)item)->vertice == (Vertice*)aux){
        vv = (VerticeValue*)item;
        return;
    }
}

VerticeValue* getVerticeValueOf(Grafo* grafo, Vertice* origem, Vertice* destino){
    if(!isInLista(grafo->listaVertices, compararV, origem) || !isInLista(grafo->listaVertices, compararV, destino)) return NULL;

    vv = NULL;
    percorrerListaRel(origem->listaDestinos, getVerticeValue, destino);

    return vv;
}


void imprimirVerticeRay(Vertice* vertice){
    DrawRectangleRec(vertice->pos, vertice->cor);
    DrawText(TextFormat("%d", vertice->id), vertice->pos.x + vertice->pos.width/2 - MeasureText(TextFormat("%d", vertice->id), 5) / 2, vertice->pos.y + vertice->pos.height/2 - (5.0f / 2.0f), 5, BLACK);
}

void imprimirArestaRay(VerticeValue* destino){
    DrawArrow((Vector2){destino->origem->pos.x, destino->origem->pos.y}, (Vector2){destino->vertice->pos.x, destino->vertice->pos.y}, 7.0f, 4.0f, 10.0f, destino->cor);

    Vector2 textF = (Vector2){(destino->origem->pos.x + (destino->vertice->pos.x - destino->origem->pos.x) * 0.25f) - MeasureText("%.1f", destino->value) / 2, ((destino->origem->pos.y + (destino->vertice->pos.y - destino->origem->pos.y) * 0.25f) - 5.0f) + 10.0f};
    
    DrawRectangle(textF.x - 0.1f, textF.y - 0.1f, MeasureText("(%.2f)", destino->value) + 0.2f, 10.0f + 0.2f, BLACK);
    DrawText(TextFormat("(%.2f)", destino->value), textF.x, textF.y, 10.0f, RED);
}

///////////////////

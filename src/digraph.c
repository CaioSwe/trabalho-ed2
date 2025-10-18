#include "digraph.h"
#include "hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// CHECAR ALOCAÇÃO
// CHECAR ALOCAÇÃO
// CHECAR ALOCAÇÃO

// CHECAR CÓPIA DE STRING (NOME)

// FAZER VERIFICAÇÃO DE CONVERSÃO E CHAR

typedef struct Vert{
    char* nome;
    Info info;
} Vert;

typedef struct EdgeStr{
    Node to;
    Info info;
} EdgeStr;

typedef struct GraphStr{
    int maxVert;

    int nVert;
    int nEdge;
    
    Hash tabelaHash;
    Vert* vertices;
    Lista* listaAdj;
} GraphStr;

Graph createGraph(int nVert){
    GraphStr* g = (GraphStr*)malloc(sizeof(GraphStr));

    g->vertices = (Vert*)malloc(nVert * sizeof(Vert));
    g->listaAdj = (Lista)malloc(nVert * sizeof(Lista));
    g->tabelaHash = criaHash(nVert, true);

    g->maxVert = nVert;

    g->nEdge = 0;
    g->nVert = 0;

    return g;
}

int getMaxNodes(Graph g){
    return ((GraphStr*)g)->maxVert;
}

int getTotalNodes(Graph g){
    return ((GraphStr*)g)->nVert;
}

Node addNode(Graph g, char* nome, Info info){
    GraphStr* graph = (GraphStr*)g;

    if(graph->nVert >= graph->maxVert){
        printf("\n- addNode() -> Máximo número de vértices alcançado (%d). -", graph->maxVert);
        return -1;
    }

    Vert* v = (Vert*)malloc(sizeof(Vert));
    
    v->nome = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(v->nome, nome);
    
    v->info = info;
    
    graph->listaAdj[graph->nVert] = criaLista();
    inserirHash(graph->tabelaHash, v->nome, graph->nVert);

    graph->vertices[graph->nVert].nome = v->nome;
    graph->vertices[graph->nVert].info = v->info;

    free(v);

    graph->nVert += 1;

    return graph->nVert - 1;
}

Node getNode(Graph g, char* nome){
    GraphStr* graph = (GraphStr*)g;

    return getHashValue(graph->tabelaHash, nome);    
}

Info getNodeInfo(Graph g, Node node){
    GraphStr* graph = (GraphStr*)g;
    
    if(node < 0 || node >= graph->nVert) return NULL;

    return graph->vertices[node].info;
}

char* getNodeName(Graph g, Node node){
    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert) return NULL;

    return graph->vertices[node].nome;
}

void setNodeInfo(Graph g, Node node, Info info){
    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert) return;

    graph->vertices[node].info = info;
}

Edge addEdge(Graph g, Node from, Node to, Info info){
    GraphStr* graph = (GraphStr*)g;

    EdgeStr* edge = (EdgeStr*)malloc(sizeof(EdgeStr));

    edge->info = info;
    edge->to = to;

    inserirFim(graph->listaAdj[from], edge);

    return edge;
}

static bool compararNodes(const void* item, const void* comparar){
    EdgeStr* edge = (EdgeStr*)item;
    const int* to = (const int*) comparar;

    return (edge->to == *to);
}

Edge getEdge(Graph g, Node from, Node to){
    GraphStr* graph = (GraphStr*)g;

    EdgeStr* edge = getItemListaI(graph->listaAdj[from], &to, compararNodes);
    
    return edge;
}

static bool compararEdges(const void* item, const void* comparar){
    const EdgeStr* edgeItem = (const EdgeStr*)item;
    const EdgeStr* edgeComp = (const EdgeStr*)comparar;

    return (edgeItem == edgeComp);
}

Node getFromNode(Graph g, Edge e){
    GraphStr* graph = (GraphStr*)g;
    EdgeStr* edge = (EdgeStr*)e;

    for(int i = 0; i < graph->nVert; i++){
        if(isInLista(graph->listaAdj[i], compararEdges, edge)) return i;
    }
    
    return -1;
}
 
Node getToNode(Graph g, Edge e){
    EdgeStr* edge = (EdgeStr*)e;
    return edge->to;
}

Info getEdgeInfo(Graph g, Edge e){
    EdgeStr* edge = (EdgeStr*)e;
    return edge->info;
}

void setEdgeInfo(Graph g, Edge e, Info info){
    EdgeStr* edge = (EdgeStr*)e;
    edge->info = info;
}

void removeEdge(Graph g, Edge e){
    // VER DEPOIS A IMPLEMENTAÇÃO PARA O TRABALHO EM ESPECÍFICO (desabilitar ou remover completamente).
}

bool isAdjacent(Graph g, Node from, Node to){
    GraphStr* graph = (GraphStr*)g;
    
    return isInLista(graph->listaAdj[from], compararNodes, &to);
}

static void* mappingGetToNode(const void* item){
    EdgeStr* edge = (EdgeStr*)item;
    return &edge->to;
}

void adjacentNodes(Graph g, Node node, Lista nosAdjacentes){
    GraphStr* graph = (GraphStr*)g;

    if(!nosAdjacentes) nosAdjacentes = criaLista();

    mapTo(graph->listaAdj[node], nosAdjacentes, mappingGetToNode);
}

void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes){
    GraphStr* graph = (GraphStr*)g;

    if(!arestasAdjacentes) arestasAdjacentes = criaLista();

    copyLista(graph->listaAdj[node], arestasAdjacentes, sizeof(EdgeStr));
}

void getNodeNames(Graph g, Lista nomesNodes){
    GraphStr* graph = (GraphStr*)g;

    if(!nomesNodes) nomesNodes = criaLista();

    for(int i = 0; i < graph->nVert; i++){
        inserirFim(nomesNodes, graph->vertices[i].nome);
    }
}

void getEdges(Graph g, Lista arestas){
    GraphStr* graph = (GraphStr*)g;

    if(!arestas) arestas = criaLista();

    for(int i = 0; i < graph->nVert; i++){
        concatLista(arestas, graph->listaAdj[i], sizeof(EdgeStr));
    }
}
 
//bool dfs(Graph g, Node node, procEdge treeEdge, forwardEdge, returnEdge, crossEdge, newTree, void *extra);

//bool bfs(Graph g, Node node, discoverNode, void *extra);

void killDG(Graph g){
    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        free(graph->vertices);
        limparLista(graph->listaAdj[i], true);
    }
    destroiHash(graph->tabelaHash);
    free(graph);
}

void percorrerGrafoRel(Graph g, void (*imprimir)(const void*, const void*), void* aux){
    GraphStr* graph = (GraphStr*)g;

    FILE* arq = (FILE*)aux;

    for(int i = 0; i < graph->nVert; i++){
        fprintf(arq, "(%-2d) %-25s -> ", i, graph->vertices[i].nome);
        percorrerListaRel(graph->listaAdj[i], imprimir, aux);
        fprintf(arq, "NULL\n");
    }
}

void getAllVerticesInfo(Graph g, Lista allVertices){
    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        inserirFim(allVertices, graph->vertices[i].info);
    }
}
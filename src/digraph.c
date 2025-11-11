#include "digraph.h"
#include "hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "fileManager.h"

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
    Node from;
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

// Cria um novo grafo.
Graph createGraph(int nVert){
    if(nVert < 0){
        printf("\n - createGraph() -> Nu'mero de vertices menor que zero. -");
        return NULL;
    }

    // Aloca espaco para o novo grafo.
    GraphStr* g = (GraphStr*)malloc(sizeof(GraphStr));
    if(checkAllocation(g, "Grafo direcionado.")) return NULL;

    // Aloca espaco para os vertices.
    g->vertices = (Vert*)malloc(nVert * sizeof(Vert));
    if(checkAllocation(g->vertices, "Lista de vertices do grafo.")){
        free(g);
        return NULL;
    }

    // Inicializa os campos do vertice no array.
    for(int i = 0; i < nVert; i++){
        g->vertices[i].nome = NULL;
        g->vertices[i].info = NULL;
    }

    // Aloca espaco para a lista de adjacencia.
    g->listaAdj = (Lista*)malloc(nVert * sizeof(Lista));
    if(checkAllocation(g->listaAdj, "Lista de adjacencia do grafo.")){
        free(g->vertices);
        free(g);
        return NULL;
    }

    // Inicializa o vetor de listas.
    for(int i = 0; i < nVert; i++){
        g->listaAdj[i] = criaLista();
    }

    // Cria a tabela hash para associacao de nome-valor
    g->tabelaHash = criaHash(nVert, true, 0.75f);

    // Seta a quantidade ma'xima de vertices, bem como os estados iniciais do nu'mero de ve'rtices e arestas.
    g->maxVert = nVert;

    g->nEdge = 0;
    g->nVert = 0;

    // Retorna o grafo criado.
    return g;
}

// Retorna o nu'mero ma'ximo de ve'rtices.
int getMaxNodes(Graph g){
    if(g == NULL){
        printf("\n - getMaxNodes() -> Grafo passado e' nulo. -");
        return -1;
    }

    return ((GraphStr*)g)->maxVert;
}

// Retorna o nu'mero de ve'rtices atuais.
int getTotalNodes(Graph g){
    if(g == NULL){
        printf("\n - getTotalNodes() -> Grafo passado e' nulo. -");
        return -1;
    }

    return ((GraphStr*)g)->nVert;
}

// Retorn o nu'mero de arestas atuais.
int getTotalEdges(Graph g){
    if(g == NULL){
        printf("\n - getTotalEdges() -> Grafo passado e' nulo. -");
        return -1;
    }

    return ((GraphStr*)g)->nEdge;
}

// Adiciona um ve'rtice no grafo com nome e informacao associada.
Node addNode(Graph g, char* nome, Info info){
    if(g == NULL || nome == NULL || info == NULL){
        printf("\n- addNode() -> Uma ou mais informacoes passadas sao nulas. -");
        return -1;
    }

    GraphStr* graph = (GraphStr*)g;

    // Se o grafo tiver mais ve'rtices que ele suporta, lanca um erro e retorna -1.
    if(graph->nVert >= graph->maxVert){
        printf("\n- addNode() -> Ma'ximo número de vértices alcançado (%d). -", graph->maxVert);
        return -1;
    }

    // Aloca espaco para o nome do ve'rtice.
    graph->vertices[graph->nVert].nome = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
    if(checkAllocation(graph->vertices[graph->nVert].nome, "Nome do novo ve'rtice do grafo.")){
        return -1;
    }
    strcpy(graph->vertices[graph->nVert].nome, nome);

    graph->vertices[graph->nVert].info = info;

    int nVert = graph->nVert;
    
    // Aloca um nVertp para a associacao do indice da lista.
    int* nVertp = (int*)malloc(sizeof(int));
    if(checkAllocation(nVertp, "Ponteiro do indice do vertice do grafo.")){
        free(graph->vertices[graph->nVert].nome);
        graph->vertices[graph->nVert].info = NULL;
        return -1;
    }

    *nVertp = nVert;

    // Guarda a informacao do indice com nome na tabela Hash do grafo.
    inserirHash(graph->tabelaHash, graph->vertices[graph->nVert].nome, nVertp);

    graph->nVert += 1;

    return nVert;
}

Node getNode(Graph g, char* nome){
    if(g == NULL || nome == NULL){
        printf("\n - getNode() -> Uma ou mais informacoes passadas sao nulas. -");
        return -1;
    }

    GraphStr* graph = (GraphStr*)g;
    int* valuep = getHashValue(graph->tabelaHash, nome);

    return (valuep ? *valuep : -1);
}

Info getNodeInfo(Graph g, Node node){
    if(g == NULL){
        printf("\n - getNodeInfo() -> Grafo passado e' nulo. -");
        return NULL;
    }

    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert){
        printf("\n - getNodeInfo() -> Node fora dos limites do grafo. -");
        return NULL;
    }
    return graph->vertices[node].info;
}

char* getNodeName(Graph g, Node node){
    if(g == NULL){
        printf("\n - getNodeName() -> Grafo passado e' nulo. -");
        return NULL;
    }

    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert){
        printf("\n - getNodeName() -> Node fora dos limites do grafo. -");
        return NULL;
    }
    return graph->vertices[node].nome;
}

void setNodeInfo(Graph g, Node node, Info info){
    if(g == NULL){
        printf("\n - setNodeInfo() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert){
        printf("\n - setNodeInfo() -> Node fora dos limites do grafo. -");
        return;
    }
    graph->vertices[node].info = info;
}

Edge addEdge(Graph g, Node from, Node to, Info info){
    if(g == NULL){
        printf("\n - addEdge() -> Grafo passado e' nulo. -");
        return NULL;
    }

    GraphStr* graph = (GraphStr*)g;
    
    if(from < 0 || from >= graph->nVert || to < 0 || to >= graph->nVert){
        printf("\n - addEdge() -> Nodes 'from' ou 'to' fora dos limites do grafo. -");
        return NULL;
    }

    EdgeStr* edge = (EdgeStr*)malloc(sizeof(EdgeStr));
    if(checkAllocation(edge, "Nova aresta do grafo.")) return NULL;

    edge->info = info;
    edge->to = to;
    edge->from = from;

    inserirFim(graph->listaAdj[from], edge);

    graph->nEdge += 1;

    return edge;
}

static bool compararNodes(Item itemO, Item itemC){
    EdgeStr* edge = (EdgeStr*)itemO;
    const int* to = (const int*)itemC;

    return (edge->to == *to);
}

Edge getEdge(Graph g, Node from, Node to){
    if(g == NULL){
        printf("\n - getEdge() -> Grafo passado e' nulo. -");
        return NULL;
    }

    GraphStr* graph = (GraphStr*)g;

    if(from < 0 || from >= graph->nVert || to < 0 || to >= graph->nVert){
        printf("\n - getEdge() -> Nodes 'from' ou 'to' fora dos limites do grafo. -");
        return NULL;
    }

    EdgeStr* edge = getItemListaI(graph->listaAdj[from], compararNodes, &to);

    return edge;
}

static bool compararEdges(Item itemO, Item itemC){
    const EdgeStr* edgeItem = (const EdgeStr*)itemO;
    const EdgeStr* edgeComp = (const EdgeStr*)itemC;

    return (edgeItem == edgeComp);
}

Node getFromNode(Graph g, Edge e){
    if(e == NULL){
        printf("\n - getToNode() -> Aresta e e' nula. -");
        return -1;
    }

    EdgeStr* edge = (EdgeStr*)e;

    return edge->from;
}
 
Node getToNode(Graph g, Edge e){
    if(e == NULL){
        printf("\n - getToNode() -> Aresta e e' nula. -");
        return -1;
    }

    EdgeStr* edge = (EdgeStr*)e;
    return edge->to;
}

Info getEdgeInfo(Graph g, Edge e){
    if(e == NULL){
        printf("\n - getEdgeInfo() -> Aresta e e' nula. -");
        return NULL;
    }

    EdgeStr* edge = (EdgeStr*)e;
    return edge->info;
}

void setEdgeInfo(Graph g, Edge e, Info info){
    if(e == NULL){
        printf("\n - setEdgeInfo() -> Aresta e e' nula. -");
        return;
    }

    EdgeStr* edge = (EdgeStr*)e;
    edge->info = info;
}

void removeEdge(Graph g, Edge e, freeFunc freeEdgeFunc){
    if(g == NULL || e == NULL){
        printf("\n - removeEdge() -> Uma ou mais informacoes passadas sao nulas. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;
    EdgeStr* edge = (EdgeStr*)e;

    Node from = getFromNode(g, e);

    if(from < 0){
        printf("\n - removeEdge() -> Node de origem nao encontrado para remocao da lista de adjacencia. -");
        return;
    }

    EdgeStr* edgeRem = remover(graph->listaAdj[from], compararEdges, edge);

    if(edgeRem != NULL){
        if(edgeRem->info != NULL && freeEdgeFunc != NULL) freeEdgeFunc(edgeRem->info, NULL);
        free(edgeRem);
    }

    graph->nEdge -= 1;
}

bool isAdjacent(Graph g, Node from, Node to){
    if(g == NULL){
        printf("\n - isAdjacent() -> Grafo passado e' nulo. -");
        return false;
    }

    GraphStr* graph = (GraphStr*)g;

    if(from < 0 || from >= graph->nVert || to < 0 || to >= graph->nVert){
        printf("\n - isAdjacent() -> Nodes 'from' ou 'to' fora dos limites do grafo. -");
        return false;
    }

    return isInLista(graph->listaAdj[from], compararNodes, &to);
}

static void* mappingGetToNode(Item item){
    EdgeStr* edge = (EdgeStr*)item;
    return &edge->to;
}

void adjacentNodes(Graph g, Node node, Lista nosAdjacentes){
    if(g == NULL){
        printf("\n - adjacentNodes() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert){
        printf("\n - adjacentNodes() -> Node fora dos limites do grafo. -");
        return;
    }

    if(nosAdjacentes == NULL) nosAdjacentes = criaLista();

    mapTo(graph->listaAdj[node], nosAdjacentes, mappingGetToNode, sizeof(int));
}

void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes){
    if(g == NULL){
        printf("\n - adjacentEdges() -> Grafo passado e' nulo. -");
        return;
    }
    
    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert){
        printf("\n - adjacentEdges() -> Node fora dos limites do grafo. -");
        return;
    }

    if(arestasAdjacentes == NULL) arestasAdjacentes = criaLista();

    copyLista(graph->listaAdj[node], arestasAdjacentes, sizeof(EdgeStr));
}

void getNodeNames(Graph g, Lista nomesNodes){
    if(g == NULL){
        printf("\n - getNodeNames() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    if(nomesNodes == NULL) nomesNodes = criaLista();

    for(int i = 0; i < graph->nVert; i++){
        inserirFim(nomesNodes, graph->vertices[i].nome);
    }
}

void getEdges(Graph g, Lista arestas){
    if(g == NULL){
        printf("\n - getEdges() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    if(arestas == NULL) arestas = criaLista();

    for(int i = 0; i < graph->nVert; i++){
        concatLista(arestas, graph->listaAdj[i], sizeof(EdgeStr));
    }
}

void getAllVerticesInfo(Graph g, Lista allInfo){
    if(g == NULL){
        printf("\n - getAllVerticesInfo() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        inserirFim(allInfo, graph->vertices[i].info);
    }
}

// ?

bool dfs(Graph g, Node node, procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, dfsRestarted newTree, void *extra){
    return false;
}

bool bfs(Graph g, Node node, procEdge discoverNode, void *extra){
    return false;
}

// ?

static void freeAresta(Edge e, void* extra){
    if(e == NULL){
        printf("\n- freeAresta() -> Aresta passada e' nula. -");
        return;
    }

    EdgeStr* edge = (EdgeStr*)e;
    void (*freeArestaInfoFunc)(Info, void*) = (void (*)(Info, void*))extra;

    freeArestaInfoFunc(edge->info, NULL);
    free(edge);
}

void killDG(Graph g, freeFunc freeVerticeFunc, freeFunc freeEdgeFunc){
    if(g == NULL){
        printf("\n - killDG() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        if(graph->vertices[i].nome) free(graph->vertices[i].nome);
        if(graph->vertices[i].info) freeVerticeFunc(graph->vertices[i].info, NULL);

        destroiLista(graph->listaAdj[i], freeAresta, freeEdgeFunc);
    }

    if (graph->listaAdj) free(graph->listaAdj);
    if (graph->vertices) free(graph->vertices);
    destroiHash(graph->tabelaHash, freeReg, NULL);
    free(graph);
}
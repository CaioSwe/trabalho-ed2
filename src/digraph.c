#include "digraph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    
    Vert* listaVert;
    Lista* listaAdj;
} GraphStr;

Graph createGraph(int nVert){
    GraphStr* g = (GraphStr*)malloc(sizeof(GraphStr));
    
    g->listaVert = malloc(nVert * sizeof(Vert));
    g->listaAdj = malloc(nVert * sizeof(Lista));
    
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
    strcpy(v->nome, nome);
    v->info = info;

    graph->listaVert[graph->nVert].nome = v->nome;
    graph->listaVert[graph->nVert].info = v->info;

    graph->listaAdj[graph->nVert] = criaLista();

    graph->nVert += 1;

    return ;
}

Node getNode(Graph g, char* nome){
    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        if(strcmp(graph->listaVert[i].nome, nome) == 0){
            return i;
        }
    }

    return -1;
}

Info getNodeInfo(Graph g, Node node);

char *getNodeName(Graph g, Node node);

void setNodeInfo(Graph g, Node node, Info info);

Edge addEdge(Graph g, Node from, Node to, Info info){
    GraphStr* graph = (GraphStr*)g;

    EdgeStr* edge = (EdgeStr*)malloc(sizeof(EdgeStr));

    edge->info = info;
    edge->to = to;

    inserirFim(graph->listaAdj[from], edge);

    return edge;
}

Edge getEdge(Graph g, Node from, Node to);

Node getFromNode(Graph g, Edge e);
 
Node getToNode(Graph g, Edge e);

Info getEdgeInfo(Graph g, Edge e);

void setEdgeInfo(Graph g, Edge e, Info info);

void removeEdge(Graph g, Edge e);

bool isAdjacent(Graph g, Node from, Node to);

void adjacentNodes(Graph g, Node node, Lista nosAdjacentes);

void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes);

void  getNodeNames(Graph g, Lista nomesNodes);

void getEdges(Graph g, Lista arestas);
 
//bool dfs(Graph g, Node node, procEdge treeEdge, forwardEdge, returnEdge, crossEdge, newTree, void *extra);

//bool bfs(Graph g, Node node, discoverNode, void *extra);

void killDG(Graph g);
#include "digraph.h"
#include "hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

#include "fileManager.h"
#include "priorityQueue.h"
#include "streap.h"

// Ve'rtice
typedef struct Vert{
    char* nome;
    Info infoNo;
} Vert;

// Aresta
typedef struct EdgeStr{
    Node to;
    Node from;
    Info infoAresta;
} EdgeStr;

// Vertice e lista de adjacencia dele.
typedef struct ListaAdj{
    Vert vertice;
    Lista listaEdges;
}ListaAdj;

// Grafo
typedef struct GraphStr{
    int maxVert;

    int nVert;
    int nEdge;
    
    Hash tabelaHash;
    ListaAdj* listaAdj; // listaAdj[Node] -> (Ve'rtice, Lista de EdgeStr* tal que from = Node)
    Lista* listaEdgeInversa; // lista[Node] de EdgeStr* tal que to = Node
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

    // Aloca espaco para a lista de adjacencia.
    g->listaAdj = (ListaAdj*)malloc(nVert * sizeof(ListaAdj));
    if(checkAllocation(g->listaAdj, "Lista de adjacencia do grafo.")){
        free(g);
        return NULL;
    }

    // Aloca espaco para a lista inversa de adjacencia.
    g->listaEdgeInversa = (Lista*)malloc(nVert * sizeof(Lista));
    if(checkAllocation(g->listaEdgeInversa, "Lista de adjacencia inversa.")){
        free(g->listaAdj);
        free(g);
        return NULL;
    }
    
    // Inicializa os campos do vertice na lista de adjacencia dele e sua lista.
    for(int i = 0; i < nVert; i++){
        g->listaAdj[i].vertice.nome = NULL;
        g->listaAdj[i].vertice.nome = NULL;

        g->listaAdj[i].listaEdges = criaLista();
        g->listaEdgeInversa[i] = criaLista();
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
    graph->listaAdj[graph->nVert].vertice.nome = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
    if(checkAllocation(graph->listaAdj[graph->nVert].vertice.nome, "Nome do novo ve'rtice do grafo.")){
        return -1;
    }
    strcpy(graph->listaAdj[graph->nVert].vertice.nome, nome);

    graph->listaAdj[graph->nVert].vertice.infoNo = info;

    int nVert = graph->nVert;
    
    // Aloca um nVertp para a associacao do indice da lista.
    int* nVertp = (int*)malloc(sizeof(int));
    if(checkAllocation(nVertp, "Ponteiro do indice do vertice do grafo.")){
        free(graph->listaAdj[graph->nVert].vertice.nome);
        graph->listaAdj[graph->nVert].vertice.infoNo = NULL;
        return -1;
    }

    *nVertp = nVert;

    // Guarda a informacao do indice com nome na tabela Hash do grafo.
    inserirHash(graph->tabelaHash, graph->listaAdj[graph->nVert].vertice.nome, nVertp);

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
    return graph->listaAdj[node].vertice.infoNo;
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
    return graph->listaAdj[node].vertice.nome;
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
    graph->listaAdj[node].vertice.infoNo = info;
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

    edge->infoAresta = info;
    edge->to = to;
    edge->from = from;

    inserirFim(graph->listaAdj[from].listaEdges, edge);
    inserirFim(graph->listaEdgeInversa[to], edge);

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

    EdgeStr* edge = getItemListaI(graph->listaAdj[from].listaEdges, compararNodes, &to);

    return edge;
}

static bool compararEdges(Item itemO, Item itemC){
    const EdgeStr* edgeItem = (const EdgeStr*)itemO;
    const EdgeStr* edgeComp = (const EdgeStr*)itemC;

    bool equals = edgeItem->from == edgeComp->from && edgeItem->to == edgeComp->to && edgeItem->infoAresta == edgeComp->infoAresta;

    return equals;
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
    return edge->infoAresta;
}

void setEdgeInfo(Graph g, Edge e, Info info){
    if(e == NULL){
        printf("\n - setEdgeInfo() -> Aresta e e' nula. -");
        return;
    }

    EdgeStr* edge = (EdgeStr*)e;
    edge->infoAresta = info;
}

void removeEdge(Graph g, Edge e, freeFunc freeEdgeFunc){
    if(g == NULL || e == NULL){
        printf("\n - removeEdge() -> Uma ou mais informacoes passadas sao nulas. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;
    EdgeStr* edge = (EdgeStr*)e;

    Node from = edge->from;
    Node to = edge->to;

    EdgeStr* edgeRem = remover(graph->listaAdj[from].listaEdges, compararEdges, edge);
    remover(graph->listaEdgeInversa[to], compararEdges, edge);

    if(edgeRem != NULL){
        if(edgeRem->infoAresta != NULL && freeEdgeFunc != NULL) freeEdgeFunc(edgeRem->infoAresta, NULL);
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

    return isInLista(graph->listaAdj[from].listaEdges, compararNodes, &to);
}

static void* mappingGetToNode(Item item, void* extra){
    EdgeStr* edge = (EdgeStr*)item;
    return &edge->to;
}

static Item copyNode(Item item){
    int* newNode = (int*)malloc(sizeof(int));
    if(checkAllocation(newNode, "Copia de node.")){
        return NULL;
    }

    return newNode;
}

static Item copyEdge(Item item){
    EdgeStr* edge = (EdgeStr*)item;

    EdgeStr* newEdge = (EdgeStr*)malloc(sizeof(EdgeStr));

    newEdge->from = edge->from;
    newEdge->infoAresta = edge->infoAresta;
    newEdge->to = edge->to;

    return newEdge;
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

    mapTo(graph->listaAdj[node].listaEdges, nosAdjacentes, mappingGetToNode, copyNode, NULL);
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

    copyLista(graph->listaAdj[node].listaEdges, arestasAdjacentes, copyEdge);
}

void adjacentEdgesReverse(Graph g, Node node, Lista arestasAdjacentes){
    if(g == NULL){
        printf("\n - adjacentEdgesReverse() -> Grafo passado e' nulo. -");
        return;
    }
    
    GraphStr* graph = (GraphStr*)g;

    if(node < 0 || node >= graph->nVert){
        printf("\n - adjacentEdgesReverse() -> Node fora dos limites do grafo. -");
        return;
    }

    if(arestasAdjacentes == NULL) arestasAdjacentes = criaLista();

    copyLista(graph->listaEdgeInversa[node], arestasAdjacentes, copyEdge);
}

void getNodeNames(Graph g, Lista nomesNodes){
    if(g == NULL){
        printf("\n - getNodeNames() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    if(nomesNodes == NULL) nomesNodes = criaLista();

    for(int i = 0; i < graph->nVert; i++){
        inserirFim(nomesNodes, graph->listaAdj[i].vertice.nome);
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
        concatLista(arestas, graph->listaAdj[i].listaEdges, sizeof(EdgeStr));
    }
}

void getAllVerticesInfo(Graph g, Lista allInfo){
    if(g == NULL){
        printf("\n - getAllVerticesInfo() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        inserirFim(allInfo, graph->listaAdj[i].vertice.infoNo);
    }
}

typedef enum Cores{
    BRANCO,
    CINZA,
    PRETO
}Cores;

// Estruturas dfs
typedef struct ResourcesDfs{
    GraphStr* graph;
    int tempo;
    int* tf;
    int* td;
    Cores* cor;
    procEdge treeEdge;
    procEdge forwardEdge;
    procEdge returnEdge;
    procEdge crossEdge;
    dfsRestarted newTree;
    void *extra;
}ResourcesDfs;

// Declaracoes de escopo
static void dfsVisit(GraphStr* graph, Node node, void* extra);
static void dfsVisitInner(Item item, void* extra);


static void dfsVisitInner(Item item, void* extra){
    ResourcesDfs* res = (ResourcesDfs*)extra;
    Edge e = (Edge)item;
    Node to = getToNode(res->graph, e);
    Node from = getFromNode(res->graph, e);

    if(res->cor[to] == BRANCO){
        if(res->treeEdge) res->treeEdge(res->graph, e, res->td[from], res->tf[to], res->extra);
        dfsVisit(res->graph, to, extra);
    }
    else if(res->cor[to] == CINZA){
        if(res->returnEdge) res->returnEdge(res->graph, e, res->td[from], res->tf[to], res->extra);
    }
    else{
        if(res->td[from] < res->td[to]){ 
            if(res->forwardEdge) res->forwardEdge(res->graph, e, res->td[from], res->tf[to], res->extra);
        }
        else if(res->crossEdge) res->crossEdge(res->graph, e, res->td[from], res->tf[to], res->extra);
    }
}

static void dfsVisit(GraphStr* graph, Node node, void* extra){
    ResourcesDfs* res = (ResourcesDfs*)extra;

    res->tempo += 1;
    res->cor[node] = CINZA;
    res->td[node] = res->tempo;

    percorrerLista(graph->listaAdj[node].listaEdges, dfsVisitInner, res);

    res->tempo += 1;
    res->cor[node] = PRETO;
    res->tf[node] = res->tempo;
}

// Busca em profundidade
bool dfs(Graph g, Node node, procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, dfsRestarted newTree, void *extra){
    if(g == NULL){
        printf("\n - dfs() -> Grafo passado e' nulo. -");
        return false;
    }
    
    GraphStr* graph = (GraphStr*)g;
    
    if(node < 0 || node >= graph->nVert){
        printf("\n - dfs() -> Node fora dos limites do grafo. -");
        return false;
    }
    
    Cores* cor = malloc(sizeof(Cores) * graph->nVert);
    int* tf = malloc(sizeof(int) * graph->nVert);
    int* td = malloc(sizeof(int) * graph->nVert);

    for(int i = 0; i < graph->nVert; i++){
        cor[i] = BRANCO;
        tf[i] = 0;
        td[i] = 0;
    }

    ResourcesDfs res = {
        graph,
        0,
        tf,
        td,
        cor,
        treeEdge,
        forwardEdge,
        returnEdge,
        crossEdge,
        newTree,
        extra
    };

    for(int n = node; n < graph->nVert; n++){
        if(cor[n] == BRANCO){
            if(newTree) newTree(graph, extra);
            dfsVisit(graph, n, &res);
        }
    }

    free(cor);
    free(tf);
    free(td);

    return true;
}

// Estruturas bfs
typedef struct ResourcesBfs{
    GraphStr* graph;
    Cores* cor;
    procEdge discoverNode;
    Lista lista;
    void* extra;
}ResourcesBfs;

static void discNodes(Item item, void* extra){
    ResourcesBfs* res = (ResourcesBfs*)extra;

    Node* nodeptr = malloc(sizeof(Node));
    checkAllocation(nodeptr, "Ponteiro para inteiro em bfs.");

    *nodeptr = getToNode(res->graph, (Edge)item);

    if(res->cor[*nodeptr] == BRANCO){
        res->cor[*nodeptr] = CINZA;
        
        inserirInicio(res->lista, nodeptr);

        if(res->discoverNode != NULL) res->discoverNode(res->graph, (Edge)item, 0, 0, res->extra);
    }
}

// Busca em largura
bool bfs(Graph g, Node node, procEdge discoverNode, void *extra){
    if(g == NULL){
        printf("\n - bfs() -> Grafo passado e' nulo. -");
        return false;
    }
    
    GraphStr* graph = (GraphStr*)g;
    
    if(node < 0 || node >= graph->nVert){
        printf("\n - bfs() -> Node fora dos limites do grafo. -");
        return false;
    }

    Cores* cor = malloc(sizeof(Cores) * graph->nVert);

    for(int i = 0; i < graph->nVert; i++){
        cor[i] = BRANCO;
    }
    
    cor[node] = CINZA;
    
    // Cria uma lista para ser usada como uma fila.
    Lista lista = criaLista();
    if(lista == NULL) return false;

    Node* nodep = (Node*)malloc(sizeof(Node));
    if(checkAllocation(nodep, "Ponteiro para inteiro em bfs.")) return false;
    *nodep = node;

    inserirInicio(lista, nodep);

    ResourcesBfs res = {
        graph,
        cor,
        discoverNode,
        lista,
        extra
    };

    while(!isListaVazia(lista)){
        Node* nodeptr = removerFim(lista);

        percorrerLista(graph->listaAdj[*nodeptr].listaEdges, discNodes, &res);
        res.cor[*nodeptr] = PRETO;
        free(nodeptr);
    }

    free(cor);

    return true;
}

static void freeAresta(Edge e, void* extra){
    if(e == NULL){
        printf("\n- freeAresta() -> Aresta passada e' nula. -");
        return;
    }

    EdgeStr* edge = (EdgeStr*)e;
    void (*freeArestaInfoFunc)(Info, void*) = (void (*)(Info, void*))extra;

    freeArestaInfoFunc(edge->infoAresta, NULL);
    free(edge);
}

void killDG(Graph g, freeFunc freeVerticeFunc, freeFunc freeEdgeFunc){
    if(g == NULL){
        printf("\n - killDG() -> Grafo passado e' nulo. -");
        return;
    }

    GraphStr* graph = (GraphStr*)g;

    for(int i = 0; i < graph->nVert; i++){
        if(graph->listaAdj[i].vertice.nome) free(graph->listaAdj[i].vertice.nome);
        if(graph->listaAdj[i].vertice.infoNo) freeVerticeFunc(graph->listaAdj[i].vertice.infoNo, NULL);

        destroiLista(graph->listaAdj[i].listaEdges, freeAresta, freeEdgeFunc);
    }

    if (graph->listaAdj) free(graph->listaAdj);
    destroiHash(graph->tabelaHash, freeReg, NULL);
    free(graph);
}

// DIJKSTRA
/////////////////////////////////////////////////////////////////

typedef struct DijkstraStr{
    Lista arestas; // Lista de EdgeStr*
    double distanciaTotal;
}DijkstraStr;

typedef struct ResourcesDijksta{
    getNumberValue getNumberFunc;
    double* distancias;
    Node* predecessores;
    bool* processados;

    PriorityQueue pq;
}ResourcesDijksta;

static bool compInt(void* item1, void* item2){
    return (*(int*)item1 == *(int*)item2);
}

static void relaxEdge(Item item, void* extra){
    EdgeStr* e = (EdgeStr*)item;
    ResourcesDijksta* res = (ResourcesDijksta*)extra;

    getNumberValue func = res->getNumberFunc;

    Node u = e->from;
    Node v = e->to;
    double dist = func(e, NULL);

    if(dist < 0){
        return;
    }

    if(res->processados[v] == false && res->distancias[v] > res->distancias[u] + dist){
        res->distancias[v] = res->distancias[u] + dist;
        res->predecessores[v] = u;

        Node* vp = (Node*)malloc(sizeof(Node));
        if(checkAllocation(vp, "Ponteiro para Node To.")){
            destroiPriorityQueue(res->pq, freeReg, NULL);
            free(res->processados);
            free(res->distancias);
            free(res->predecessores);
        }
        *vp = v;

        // insere se nao existe, muda a prioridade caso exista.
        if(changePriorityQueue(res->pq, vp, res->distancias[v], compInt)) free(vp);
    }
}

Caminho getShortestPath(Graph g, Node from, Node to, getNumberValue getNumberFunc){
    if(g == NULL){
        printf("\n - getShortestPath() -> Grafo passado e' nulo. -");
        return NULL;
    }

    if(getNumberFunc == NULL){
        printf("\n - getShortestPath() -> Funcao numerica passada e' nula. -");
        return NULL;
    }

    GraphStr* graph = (GraphStr*)g;

    if(from < 0 || from >= graph->nVert || to < 0 || to >= graph->nVert){
        printf("\n - getShortestPath() -> Algum dos nodes esta' fora dos limites do grafo. -");
        return NULL;
    }

    double* distancias = (double*)malloc(sizeof(double) * graph->nVert);
    if(checkAllocation(distancias, "Vetor das distancias (Dijkstra).")) return NULL;
    
    Node* predecessores = (Node*)malloc(sizeof(Node) * graph->nVert);
    if(checkAllocation(predecessores, "Vetor dos predecessores (Dijkstra).")){
        free(distancias);
        return NULL;
    }

    bool* processados = (bool*)malloc(sizeof(bool) * graph->nVert);
    if(checkAllocation(processados, "Vetor dos processados (Dijkstra).")){
        free(predecessores);
        free(distancias);
        return NULL;
    }

    PriorityQueue filaDeProcessamento = criaPriorityQueue(graph->nVert);

    Node* fromP = malloc(sizeof(Node));
    if(checkAllocation(fromP, "Ponteiro para Node from (Dijkstra).")){
        free(processados);
        free(predecessores);
        free(distancias);
        return NULL;
    }

    *fromP = from;
    inserirPriorityQueue(filaDeProcessamento, fromP, 0.0f);

    for(int i = 0; i < graph->nVert; i++){
        processados[i] = false;
        predecessores[i] = -1;
        distancias[i] = DBL_MAX;
    }

    distancias[from] = 0;

    ResourcesDijksta res = {
        getNumberFunc,
        distancias,
        predecessores,
        processados,
        filaDeProcessamento
    };

    while(!isPriorityQueueVazia(filaDeProcessamento)){
        Node* u = removerMinPriorityQueue(filaDeProcessamento);
        processados[*u] = true;

        if(*u == to){
            free(u);
            break;
        }

        percorrerLista(graph->listaAdj[*u].listaEdges, relaxEdge, &res);
        free(u);
    }

    destroiPriorityQueue(filaDeProcessamento, freeReg, NULL);
    
    // Impossivel de chegar ao destino.
    if(predecessores[to] == -1){
        // printf("\n - getShortestPath() -> Caminho impossivel de alcancar. -");

        free(distancias);
        free(predecessores);
        free(processados);
        return NULL;
    }

    // Caminho minimo adiquirido.
    // Adaptando arrays para retorno.

    DijkstraStr* dStr = (DijkstraStr*)malloc(sizeof(DijkstraStr));
    if(checkAllocation(dStr, "Dijkstra.")){
        free(distancias);
        free(predecessores);
        free(processados);
        return NULL;
    }

    dStr->arestas = criaLista();

    for(int i = to; i != from; i = predecessores[i]){
        Edge e = getEdge(graph, predecessores[i], i);

        inserirInicio(dStr->arestas, e);
    }

    dStr->distanciaTotal = distancias[to];

    // printf("\n Caminho adiquirido com sucesso.");

    free(distancias);
    free(predecessores);
    free(processados);

    return dStr;
}

Lista getDijkstraList(Caminho caminho){
    if(caminho == NULL){
        printf("\n - getDijkstraList() -> caminho passado e' nulo. -");
        return NULL;
    }

    DijkstraStr* djk = (DijkstraStr*)caminho;

    return djk->arestas;
}

double getDijkstraDistance(Caminho caminho){
    if(caminho == NULL){
        printf("\n - getDijkstraDistance() -> caminho passado e' nulo. -");
        return -1.0f;
    }

    DijkstraStr* djk = (DijkstraStr*)caminho;

    return djk->distanciaTotal;
}

void freeCaminho(Caminho caminho){
    if(caminho == NULL){
        printf("\n - freeCaminho() -> caminho passado e' nulo. -");
        return;
    }

    DijkstraStr* djk = (DijkstraStr*)caminho;

    destroiLista(djk->arestas, NULL, NULL);
    free(djk);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "geo.h"
#include "via.h"

// MUDAR TAMANHO DA TABELA HASH DOS COMANDOS DE BLOQUEIO PARA ADAPTABILIDADE.

// Estrutura de ponto da cidade que contem No', face e numero.
typedef struct Ponto{
    Node node;
    char face;
    int num;
}Ponto;

// Estrutura de percurso com as informacoes do percurso associado.
typedef struct PercursoStr{
    Ponto* origem;
    Ponto* destino;

    char cmc[256];
    char cmr[256];

    Caminho caminhoCurto;
    Caminho caminhoRapido;
}PercursoStr;

// Estrutura a ser armazenada nas listas da tabela Hash para os comandos [blq, rbl].
typedef struct BloqueioStr{
    Lista lista; // Lista das arestas desabilitadas [Edge]
    char* bloqueio; // Comando de bloqueio [sn, ns, lo, ol]
}BloqueioStr;

static void freeBloqueio(void* bloqueio, void* extra){
    BloqueioStr* bloqueiostr = (BloqueioStr*)bloqueio;

    destroiLista(bloqueiostr->lista, freeReg, NULL);
    free(bloqueiostr->bloqueio);
}

typedef struct ResourcesSentido{
    Graph graph;
    Lista lista;
    char* sentido;
}ResourcesSentido;

static void verificarSentidoBlock(Item item, void* extra){
    Edge e = (Edge)item;
    ResourcesSentido* res = (ResourcesSentido*)extra;

    Node from = getFromNode(res->graph, e);
    Node to = getToNode(res->graph, e);

    Info infoFrom = getNodeInfo(res->graph, from);
    Info infoTo = getNodeInfo(res->graph, to);

    Info edgeInfo = getEdgeInfo(res->graph, e);

    if(strcmp(res->sentido, "ns") == 0){
        double y1 = getVerticeViaY(infoFrom);
        double y2 = getVerticeViaY(infoTo);

        if(y1 < y2) blockVia(edgeInfo);
        inserirFim(res->lista, e);
    }
    else if(strcmp(res->sentido, "sn") == 0){
        double y1 = getVerticeViaY(infoFrom);
        double y2 = getVerticeViaY(infoTo);

        if(y1 > y2) blockVia(edgeInfo);
        inserirFim(res->lista, e);
    }
    else if(strcmp(res->sentido, "lo") == 0){
        double x1 = getVerticeViaX(infoFrom);
        double x2 = getVerticeViaX(infoTo);

        if(x1 > x2) blockVia(edgeInfo);
        inserirFim(res->lista, e);
    }
    else if(strcmp(res->sentido, "ol") == 0){
        double x1 = getVerticeViaX(infoFrom);
        double x2 = getVerticeViaX(infoTo);

        if(x1 < x2) blockVia(edgeInfo);
        inserirFim(res->lista, e);
    }
}

static void unblockViaVoid(Item item, void* extra){
    ResourcesSentido* res = (ResourcesSentido*)extra;
    Edge e = (Edge)item;
    
    Info edgeInfo = getEdgeInfo(res->graph, e);
    
    unblockVia(edgeInfo);
}

// Desabilita todas as arestas passadas para esta funcao.
// Possíveis valores de sentido: ns (Norte-Sul), sn (Sul-Norte), lo (Leste-Oeste), ol (Oeste-Leste).
static void bloquearNodeSentido(Item item, void* extra){
    ResourcesSentido* res = (ResourcesSentido*)extra;
    Node* nodep = (Node*)item;

    Lista adjacentes = criaLista();

    adjacentEdges(res->graph, *nodep, adjacentes);

    percorrerLista(adjacentes, verificarSentidoBlock, res);
}

// Habilita todas as arestas passadas para esta funcao.
static void desbloquearSentido(Item item, void* extra){
    ResourcesSentido* res = (ResourcesSentido*)extra;
    Node* nodep = (Node*)item;

    Lista adjacentes = criaLista();

    adjacentEdges(res->graph, *nodep, adjacentes);

    percorrerLista(adjacentes, unblockViaVoid, res);
}

static void removerQuadraLista(Item item, void* extra){
    Quadra q = (Quadra)item;
    // Quadras qs = (Quadras)extra;

    printf("\nQ = (%s)", getQuadraID(q));
    setQuadraCFill(q, "#660080");

    //removerQuadra(qs, q);
}

// Aumenta a velocidade media das arestas passadas.
static bool aumentarVmArestas(Graph g, Edge e, int td, int tf, void* extra){
    Info info = getEdgeInfo(g, e);
    setArestaVM(info, *(double*)extra);
    setEdgeInfo(g, e, info);

    return true;
}

// Funcao para pegar a velocidade media de uma aresta na lista (Dijkstra).
double getArestaVMVoid(Edge e, void* extra){
    return getArestaVM(getEdgeInfo(NULL, e));
}

// Funcao para pegar o comprimento de uma aresta na lista (Dijkstra).
double getArestaCMPVoid(Edge e, void* extra){
    return getArestaCMP(getEdgeInfo(NULL, e));
}

typedef struct ResourcesInsertSTrpGraphVoid{
    STreap grafoStrp;
    Graph grafo;
}ResourcesInsertSTrpGraphVoid;

static void insertSTrpGraphVoid(Item item, void* extra){
    ResourcesInsertSTrpGraphVoid* res = (ResourcesInsertSTrpGraphVoid*)extra;
    char* nome = (char*)item;
    
    Node node = getNode(res->grafo, nome);
    Info nodeInfo = getNodeInfo(res->grafo, node);
    double x = getVerticeViaX(nodeInfo);
    double y = getVerticeViaY(nodeInfo);

    Node* nodep = (Node*)malloc(sizeof(Node));
    if(checkAllocation(nodep, "Ponteiro para node na insercao do grafo STrp.")){
        killSTrp(res->grafoStrp, NULL, NULL);
        return;
    }

    insertSTrp(res->grafoStrp, x, y, nodep);
}

Percurso processQryFile(Graph grafo, Quadras quadras, const char* path){
    // Checa se o caminho dado contam a extensao .qry
    if(strstr(path, ".qry") == NULL){
        printf("\n- processQryFile() -> path: \"%s\" nao e' um arquivo .qry -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    // Variaveis utilizadas na leitura do QRY:
    // Operacao.
    char op[256];
    
    // Cep, face e numero.
    char cep[256];
    char face;
    int num;
    
    // Quadrado [x, y, width, height].
    double x, y, w, h;

    // Cor de preenchimento e cor de borda.
    char cfill[256];
    char cstrk[256];

    // Nome e sentido.
    char nome[256];
    char sentido[2];

    // Fator escalar.
    double fator;

    // Cores do caminho mais curto e caminho mais ra'pido.
    char cmc[256];
    char cmr[256];

    // Cria uma tabela Hash para a associacao de nome-lista para os comandos de bloqueio de remocao de bloqueio.
    Hash tabelaHash = criaHash(50, true, 0.75f);

    // Cria uma STreap para busca de nodes dentro de uma area [catac, blq, b].
    STreap grafoSTreap = createSTrp(0.000001);

    // Insere todos os nomes dos nodes do grafo na STreap.
    Lista nomesGrafo = criaLista();
    getNodeNames(grafo, nomesGrafo);
    percorrerLista(nomesGrafo, insertSTrpGraphVoid, &(ResourcesInsertSTrpGraphVoid){grafoSTreap, grafo});

    // Cria e inicializa a estrutura de percurso.
    PercursoStr* percurso = (PercursoStr*)malloc(sizeof(PercursoStr));
    if(checkAllocation(percurso, "Estrutura de percurso.")){
        destroiHash(tabelaHash, freeBloqueio, NULL);
        return NULL;
    }

    percurso->origem = NULL;
    percurso->destino = NULL;
    percurso->caminhoCurto = NULL;

    int in = 0;

    // Itera sobre as linhas do arquivo .qry
    while(fscanf(fEntrada, "%s", op) > 0){
        printf("\nLendo [%d] operacao.", in);
        in += 1;

        if(strcmp(op, "@o?") == 0){
            // Pega [cep, face, num] da operacao de origem.
            fscanf(fEntrada, "%s %c %d\n", cep, &face, &num);

            // Pega o node com o cep registrado no grafo.
            // Node node = getNode(grafo, cep);
            printf("\n a");

            percurso->origem = (Ponto*)malloc(sizeof(Ponto));
            if(checkAllocation(percurso->origem, "Ponto de origem do percurso.")){
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return NULL;
            }

            // Registra as informacoes do node encontrado na origem do percurso.
            //percurso->origem->node = node;
            percurso->origem->node = 250;
            percurso->origem->face = face;
            percurso->origem->num = num;
        }
        else if(strcmp(op, "catac") == 0){
            // Pega [x, y, w, h] da operacao de cataclisma.
            fscanf(fEntrada, "%lf %lf %lf %lf\n", &x, &y, &w, &h);

            Lista lista = criaLista();

            // Pega a lista das quadras dentro da regiao [x, y, w, h].
            getQuadrasRegion(quadras, x, y, w, h, lista);
            // Percorre a lista das quadras dentro da regiao, excluindo-as.
            percorrerLista(lista, removerQuadraLista, quadras);

            limparLista(lista, NULL, NULL);
        }
        else if(strcmp(op, "pnt") == 0){
            // Pega [cep, cfill, cstrk] da operacao de paint.
            fscanf(fEntrada, "%s %s %s\n", cep, cfill, cstrk);

            // Pega a quadra associada ao cep
            Quadra quadra = getQuadraByID(quadras, cep);

            // Muda as propriedades da quadra
            setQuadraCFill(quadra, cfill);
            setQuadraCStrk(quadra, cstrk);
        }
        else if(strcmp(op, "blq") == 0){
            // Pega [nome, sentido, x, y, w, h] da operacao de bloqueio.
            fscanf(fEntrada, "%s %s %lf %lf %lf %lf\n", nome, sentido, &x, &y, &w, &h);
        
            Lista lista = criaLista();
            Lista listaDeBloqueios = criaLista();

            // Pega os nodes dentro da regiao [x, y, w, h].
            getNodeRegiaoSTrp(grafoSTreap, x, y, w, h, lista);

            // Percorre a lista dos nodes encontrados, bloqueando as arestas que estao no sentido fornecido.
            percorrerLista(lista, bloquearNodeSentido, &(ResourcesSentido){grafo, listaDeBloqueios, sentido});

            // Inicializa a estrutura de bloqueios a ser armazenada na tabela Hash.
            BloqueioStr* listaBlq = (BloqueioStr*)malloc(sizeof(BloqueioStr));
            if(checkAllocation(listaBlq, "Estrutura de bloqueios.")){
                if(percurso->origem) free(percurso->origem);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return NULL;
            }

            listaBlq->bloqueio = (char*)malloc(sizeof(char) * strlen(sentido));
            if(checkAllocation(listaBlq->bloqueio, "String de sentido do bloqueio.")){
                if(percurso->origem) free(percurso->origem);
                free(listaBlq);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return NULL;
            }

            strcpy(listaBlq->bloqueio, sentido);
            
            listaBlq->lista = listaDeBloqueios;

            // Insere a estrutura BloqueioStr (Lista lista, char* sentido) na tabela Hash para busca do comando [rbl].
            inserirHash(tabelaHash, nome, listaBlq);
        }
        else if(strcmp(op, "rbl") == 0){
            // Pega [nome] da operacao de remocao de bloqueio.
            fscanf(fEntrada, "%s\n", nome);
        
            // Busca a estrutura de Bloqueio associada ao nome na tabela Hash.
            BloqueioStr* blqs = (BloqueioStr*)getHashValue(tabelaHash, nome);

            percorrerLista(blqs->lista, desbloquearSentido, blqs->bloqueio);
        }
        else if(strcmp(op, "b") == 0){
            // Pega [x, y, fator] da operacao de boost(?)
            fscanf(fEntrada, "%lf %lf %lf\n", &x, &y, &fator);

            // Pega o node mais pro'ximo das coordenadas.
            // Node node = getClosestNode(grafo, x, y);

            // Comeca o percurso em largura do node.
            bfs(grafo, 0, aumentarVmArestas, &fator);
        }
        else if(strcmp(op, "p?") == 0){
            // Pega [cep, face, num, cmc, cmr] da operacao de percurso.
            fscanf(fEntrada, "%s %c %d %s %s\n", cep, &face, &num, cmc, cmr);

            if(percurso->origem == NULL){
                printf("\n- processQryFile() -> ponto de origem nao definido antes da operacao 'p?'. -");
                if(percurso->origem) free(percurso->origem);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return NULL;
            }

            percurso->destino = (Ponto*)malloc(sizeof(Ponto));
            if(checkAllocation(percurso->destino, "Ponto de destino do percurso.")){
                if(percurso->origem) free(percurso->origem);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return NULL;
            }

            // Associa todos os valores passados ao destino.
            // percurso->destino->node = getNode(grafo, cep);
            percurso->destino->node = 50;
            percurso->destino->face = face;
            percurso->destino->num = num;

            strcpy(percurso->cmc, cmc);
            strcpy(percurso->cmr, cmr);

            // Calcula o caminho mais curto e o caminho mais ra'pido com Dijkstra (implementacao no final de digraph.c).
            percurso->caminhoCurto = getShortestPath(grafo, percurso->origem->node, percurso->destino->node, getArestaCMPVoid);
            percurso->caminhoRapido = getShortestPath(grafo, percurso->origem->node, percurso->destino->node, getArestaVMVoid);
        
            // Caso algum dos caminhos for nulo, aborta o retorno do percurso.
            if(percurso->caminhoCurto == NULL || percurso->caminhoRapido == NULL){
                if(percurso->caminhoCurto != NULL) freeCaminho(percurso->caminhoCurto, freeArestaVia);
                if(percurso->caminhoRapido != NULL) freeCaminho(percurso->caminhoRapido, freeArestaVia);

                free(percurso->origem);
                free(percurso->destino);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);

                return NULL;
            }
        }
    }

    printf("\n");

    // Fecha o arquivo de entrada
    fclose(fEntrada);

    return percurso;
}

Caminho getCaminhoCurto(Percurso percurso){
    PercursoStr* p = (PercursoStr*)percurso;
    return p->caminhoCurto;
}

Caminho getCaminhoRapido(Percurso percurso){
    PercursoStr* p = (PercursoStr*)percurso;
    return p->caminhoRapido;
}

const char* getPercursoRapidoColor(Percurso percurso){
    PercursoStr* p = (PercursoStr*)percurso;
    return p->cmr;
}

const char* getPercursoCurtoColor(Percurso percurso){
    PercursoStr* p = (PercursoStr*)percurso;
    return p->cmc;
}
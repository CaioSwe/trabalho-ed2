#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "lista.h"
#include "streap.h"
#include "files.h"

#include "qry.h"
#include "via.h"

// ANIMACAO
//////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct ResourcesAnimation{
    Graph grafo;
    FILE* file;

    double* vm;
    double placement;
}ResourcesAnimation;

void animateEdgeSVG(Item item, void* extra){
    Edge e = (Edge)item;
    ResourcesAnimation* res = (ResourcesAnimation*)extra;

    Node f = getFromNode(NULL, e);
    Node t = getToNode(NULL, e);

    Info infoF = getNodeInfo(res->grafo, f);
    Info infoT = getNodeInfo(res->grafo, t);

    double x1 = getVerticeViaX(infoF) - res->placement;
    double y1 = getVerticeViaY(infoF) - res->placement;

    double x2 = getVerticeViaX(infoT) - res->placement;
    double y2 = getVerticeViaY(infoT) - res->placement;

    double vm = getArestaVM(getEdgeInfo(res->grafo, e));

    *res->vm += vm;

    fprintf(res->file, " %.1f,%.1f", x1, y1);
    fprintf(res->file, " %.1f,%.1f", x2, y2);
}

void animatePercurso(Graph grafo, Lista listaEdges, const char* color, FILE* file){
    static int pathID = 0;
    double vm = 0;

    fprintf(file, "\n<path id=\"path%d\" d=\"M", pathID);
    percorrerLista(listaEdges, animateEdgeSVG, &(ResourcesAnimation){grafo, file, &vm, pathID * 5});

    fprintf(file, "\" fill=\"none\" stroke=\"%s\" stroke-width=\"4\"/>", color);
    fprintf(file, "\n<circle r=\"10\" fill=\"%s\"><animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" rotate=\"auto\"><mpath href=\"#path%d\"/></animateMotion></circle>\n", color, vm, pathID);

    pathID += 1;
}

// PERCURSO
//////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Coords{
    double x;
    double y;
}Coords;

// Estrutura de ponto da cidade que contem No', face e numero.
typedef struct Ponto{
    Node node;
    Coords coords;
    char face;
    int num;
}Ponto;

// Estrutura de percurso com as informacoes do percurso associado.
typedef struct PercursoStr{
    Ponto* origem;
    Ponto* destino;

    char cmc[256];
    char cmr[256];

    Caminho caminhoCurto; // Lista de Edges + distancia (CMP)
    Caminho caminhoRapido; // Lista de Edges + tempo (VM)
}PercursoStr;

// Estrutura a ser armazenada nas listas da tabela Hash para os comandos [blq, rbl].
typedef struct BloqueioStr{
    char* nome; // Nome do bloqueio
    Lista lista; // Lista das arestas desabilitadas [Edge]
}BloqueioStr;

static void freeBloqueio(void* bloqueio, void* extra){
    BloqueioStr* bloqueiostr = (BloqueioStr*)bloqueio;

    destroiLista(bloqueiostr->lista, freeReg, NULL);
    free(bloqueiostr->nome);
    free(bloqueiostr);
}

typedef struct ResourcesSentido{
    Graph g;
    STreap gs;
    Lista lista;
    char* sentido;
    FILE* txt;
}ResourcesSentido;

static void verificarSentidoBlock(Item item, void* extra){
    Edge e = (Edge)item;
    ResourcesSentido* res = (ResourcesSentido*)extra;

    Node from = getFromNode(res->g, e);
    Node to = getToNode(res->g, e);

    Info infoFrom = getNodeInfo(res->g, from);
    Info infoTo = getNodeInfo(res->g, to);

    Info edgeInfo = getEdgeInfo(res->g, e);

    if(strcmp(res->sentido, "sn") == 0){
        double y1 = getVerticeViaY(infoFrom);
        double y2 = getVerticeViaY(infoTo);

        if(y1 < y2){
            blockVia(edgeInfo);
            fprintf(res->txt, " <-> Aresta Desabilitada | Nome = %-20s | Origem = %-20s | Destino = %-20s | Velocidade Media = %4.1f | Comprimento = %4.1f\n", getArestaViaNome(edgeInfo), getNodeName(res->g, getFromNode(res->g, e)), getNodeName(res->g, getToNode(res->g, e)), getArestaVM(edgeInfo), getArestaCMP(edgeInfo));
            inserirFim(res->lista, e);
        }
    }
    else if(strcmp(res->sentido, "ns") == 0){
        double y1 = getVerticeViaY(infoFrom);
        double y2 = getVerticeViaY(infoTo);

        if(y1 > y2){
            blockVia(edgeInfo);
            fprintf(res->txt, " <-> Aresta Desabilitada | Nome = %-20s | Origem = %-20s | Destino = %-20s | Velocidade Media = %4.1f | Comprimento = %4.1f\n", getArestaViaNome(edgeInfo), getNodeName(res->g, getFromNode(res->g, e)), getNodeName(res->g, getToNode(res->g, e)), getArestaVM(edgeInfo), getArestaCMP(edgeInfo));
            inserirFim(res->lista, e);
        }
    }
    else if(strcmp(res->sentido, "ol") == 0){
        double x1 = getVerticeViaX(infoFrom);
        double x2 = getVerticeViaX(infoTo);

        if(x1 > x2){
            blockVia(edgeInfo);
            fprintf(res->txt, " <-> Aresta Desabilitada | Nome = %-20s | Origem = %-20s | Destino = %-20s | Velocidade Media = %4.1f | Comprimento = %4.1f\n", getArestaViaNome(edgeInfo), getNodeName(res->g, getFromNode(res->g, e)), getNodeName(res->g, getToNode(res->g, e)), getArestaVM(edgeInfo), getArestaCMP(edgeInfo));
            inserirFim(res->lista, e);
        }
    }
    else if(strcmp(res->sentido, "lo") == 0){
        double x1 = getVerticeViaX(infoFrom);
        double x2 = getVerticeViaX(infoTo);

        if(x1 < x2){
            blockVia(edgeInfo);
            fprintf(res->txt, " <-> Aresta Desabilitada | Nome = %-20s | Origem = %-20s | Destino = %-20s | Velocidade Media = %4.1f | Comprimento = %4.1f\n", getArestaViaNome(edgeInfo), getNodeName(res->g, getFromNode(res->g, e)), getNodeName(res->g, getToNode(res->g, e)), getArestaVM(edgeInfo), getArestaCMP(edgeInfo));
            inserirFim(res->lista, e);
        }
    }
}

// Desabilita todas as arestas passadas para esta funcao.
// Possíveis valores de sentido: ns (Norte-Sul), sn (Sul-Norte), lo (Leste-Oeste), ol (Oeste-Leste).
static void bloquearNodeSentido(Item item, void* extra){
    ResourcesSentido* res = (ResourcesSentido*)extra;
    NodeST nodeSt = (NodeST)item;

    Info infoStrp = getInfoSTrp(NULL, nodeSt, NULL, NULL, NULL, NULL, NULL, NULL);
    Node* nodep = (Node*)infoStrp;

    Lista adjacentes = criaLista();

    adjacentEdges(res->g, *nodep, adjacentes);

    percorrerLista(adjacentes, verificarSentidoBlock, res);
    limparLista(adjacentes, NULL, NULL);
}

typedef struct RousourcesRBL{
    Graph g;
    FILE* txt;
}RousourcesRBL;

// Habilita todas as arestas passadas para esta funcao.
static void desbloquearSentido(Item item, void* extra){
    RousourcesRBL* res = (RousourcesRBL*)extra;
    Edge e = (Edge)item;

    Info edgeInfo = getEdgeInfo(res->g, e);
    
    unblockVia(edgeInfo);
    fprintf(res->txt, " <+> Aresta Reabilitada | Nome = %-20s | Origem = %-20s | Destino = %-20s | Velocidade Media = %4.1f | Comprimento = %4.1f\n", getArestaViaNome(edgeInfo), getNodeName(res->g, getFromNode(res->g, e)), getNodeName(res->g, getToNode(res->g, e)), getArestaVM(edgeInfo), getArestaCMP(edgeInfo));
}

typedef struct ResourcesRemoverQuadra{
    Quadras qs;
    FILE* txt;
}ResourcesRemoverQuadra;

static void removerQuadraLista(Item item, void* extra){
    Quadra q = (Quadra)item;
    ResourcesRemoverQuadra* res = (ResourcesRemoverQuadra*)extra;
    
    // Reporta no txt
    fprintf(res->txt, " <!> Quadra Removida | ID = %s | Posicao = (%.1f, %.1f)\n", getQuadraID(q), getQuadraX(q), getQuadraY(q));
    
    // Remove a quadra completamente da memo'ria.
    removerQuadra(res->qs, q);
}

typedef struct ResourcesRemoverAresta{
    Graph g;
    FILE* txt;
}ResourcesRemoverAresta;

static void removerArestaVoid(Item item, void* extra){
    Edge e = (Edge)item;
    ResourcesRemoverAresta* res = (ResourcesRemoverAresta*)extra;

    Info edgeInfo = getEdgeInfo(res->g, e);

    // Reporta no txt
    fprintf(res->txt, " <!> Aresta Removida | Nome = %-20s | Origem = %-20s | Destino = %-20s | Velocidade Media = %4.1f | Comprimento = %4.1f\n", getArestaViaNome(edgeInfo), getNodeName(res->g, getFromNode(res->g, e)), getNodeName(res->g, getToNode(res->g, e)), getArestaVM(edgeInfo), getArestaCMP(edgeInfo));

    // Remove a aresta completamente do grafo.
    removeEdge(res->g, e, freeArestaVia);
}

typedef struct ResourcesEdgeRem{
    Graph grafo;
    STreap streap;
    FILE* txt;
}ResourcesEdgeRem;

static void removerArestasListaeSTreap(Item item, void* extra){
    NodeST nodeSt = (NodeST)item;
    ResourcesEdgeRem* res = (ResourcesEdgeRem*)extra;

    Info nodeInfo = deleteNodeSTrp(res->streap, nodeSt);
    Node* nodeG = (Node*)nodeInfo;
    
    Lista arestasFrom = criaLista();
    Lista arestasTo = criaLista();

    adjacentEdges(res->grafo, *nodeG, arestasFrom);
    adjacentEdgesReverse(res->grafo, *nodeG, arestasTo);

    percorrerLista(arestasFrom, removerArestaVoid, &(ResourcesRemoverAresta){res->grafo, res->txt});
    percorrerLista(arestasTo, removerArestaVoid, &(ResourcesRemoverAresta){res->grafo, res->txt});
    
    limparLista(arestasFrom, NULL, NULL);
    limparLista(arestasTo, NULL, NULL);

    free(nodeG);
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
    Info edgeInfo = getEdgeInfo(NULL, e);
    return isArestaEnabled(edgeInfo) ? getArestaVM(edgeInfo) : -1.0;
}

// Funcao para pegar o comprimento de uma aresta na lista (Dijkstra).
double getArestaCMPVoid(Edge e, void* extra){
    Info edgeInfo = getEdgeInfo(NULL, e);
    return isArestaEnabled(edgeInfo) ? getArestaCMP(edgeInfo) : -1.0;
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

    *nodep = node;

    insertSTrp(res->grafoStrp, x, y, nodep);
}

Coords calculateCoordsByQuadraFaceNum(Quadra quadra, char face, int num){
    double quadraX = getQuadraX(quadra);
    double quadraY = getQuadraY(quadra);
    double quadraWidth = getQuadraWidth(quadra);
    double quadraHeight = getQuadraHeight(quadra);

    double pontoX = 0;
    double pontoY = 0;

    switch (face){
    case 'S':
        if(num > quadraWidth){
            printf("\n getCoordsByQuadraFaceNum() -> Numero da quadra invalido.");
            return (Coords){0, 0};
        }

        pontoX = quadraX + num;
        pontoY = quadraY;

        break;
    case 'N':
        if(num > quadraWidth){
            printf("\n getCoordsByQuadraFaceNum() -> Numero da quadra invalido.");
            return (Coords){0, 0};
        }

        pontoY = quadraY + quadraHeight;
        pontoX = quadraX + num;

        break;
    case 'L':
        if(num > quadraHeight){
            printf("\n getCoordsByQuadraFaceNum() -> Numero da quadra invalido.");
            return (Coords){0, 0};
        }

        pontoY = quadraY + num;
        pontoX = quadraX;

        break;
    case 'O':
        if(num > quadraHeight){
            printf("\n getCoordsByQuadraFaceNum() -> Numero da quadra invalido.");
            return (Coords){0, 0};
        }

        pontoX = quadraX + quadraWidth;
        pontoY = quadraY + num;

        break;
    default:
        printf("\n getCoordsByQuadraFaceNum() -> Face invalida.");
        return (Coords){0, 0};
    }

    return (Coords){pontoX, pontoY};
}

typedef struct ResourcesDescreverCaminhoTXT{
    Graph g;
    FILE* txt;
    Node origem;
    Node destino;
    double* cmp;
    double* vm;
    
    char* lastDirection;
}ResourcesDescreverCaminhoTXT;

char* getSentidoDescPath(Graph g, Node f, Node t, double epsilon){
    Info infoF = getNodeInfo(g, f);
    Info infoT = getNodeInfo(g, t);

    double x1 = getVerticeViaX(infoF);
    double y1 = getVerticeViaY(infoF);
    double x2 = getVerticeViaX(infoT);
    double y2 = getVerticeViaY(infoT);

    if(!(fabs(x1 - x2) <= epsilon)){
        if(x1 < x2) return "oeste";
        if(x1 > x2) return "leste";
    }
    if(!(fabs(y1 - y2) <= epsilon)){
        if(y1 < y2) return "norte";
        if(y1 > y2) return "sul";
    }

    return "?";
}

// SVG
//////////////////////////////////////////////////////////////////////////////////////////////////

static void descreverPathingAndCount(Item item, void* extra){
    Edge e = (Edge)item;
    ResourcesDescreverCaminhoTXT* res = (ResourcesDescreverCaminhoTXT*)extra;

    Node from = getFromNode(res->g, e);
    Node to = getToNode(res->g, e);

    Info edgeInfo = getEdgeInfo(res->g, e);
    const char* nomeAresta = getArestaViaNome(edgeInfo);

    char* direction = getSentidoDescPath(res->g, from, to, 0.000001);

    if(res->origem == from){
        fprintf(res->txt, "\n <o> Comecando do CEP %s...", getNodeName(res->g, from));

        fprintf(res->txt, "\n Siga na direcao %s da rua %s ate o ", direction, nomeAresta);
        res->lastDirection = direction;
    }

    if(strcmp(res->lastDirection, direction) != 0){
        res->lastDirection = direction;
        fprintf(res->txt, "cruzamento da rua %s.", nomeAresta);
        fprintf(res->txt, "\n Vire para a direcao %s e siga reto ate o ", direction);
    }

    if(res->destino == to){
        fprintf(res->txt, "destino.");
        fprintf(res->txt, "\n <p> Percurso finalizado no CEP %s.\n", getNodeName(res->g, to));
    }

    *res->cmp += getArestaCMP(edgeInfo);
    *res->vm += getArestaVM(edgeInfo);
}

static void descreverCaminhoTXT(FILE* txt, const char* tipoDeCaminho, Lista edgeLista, Graph g, Node origem, Node destino){
    fprintf(txt, "\n <#> Descricao textual do caminho: %s", tipoDeCaminho);

    double cmp = 0.0;
    double vm = 0.0;
    percorrerLista(edgeLista, descreverPathingAndCount, &(ResourcesDescreverCaminhoTXT){g, txt, origem, destino, &cmp, &vm, ""});
    fprintf(txt, "\n <~> Distancia total | Comprimento = %5.1f | Velocidade Media = %5.1f\n\n", cmp, vm);
}

static void linhaOrigemSvg(FILE* svg, double x, double y, const char* color){
    fprintf(svg, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" style=\"stroke:%s;stroke-width:4\"/>\n", x, y, x, 0.0, color);
    fprintf(svg, "<text x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" dominant-baseline=\"hanging\">origem</text>", x, 0.0, color);
}

static void linhaDestinoSvg(FILE* svg, double x, double y, const char* color){
    fprintf(svg, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" style=\"stroke:%s;stroke-width:4\"/>\n", x, y, 0.0, y, color);
    fprintf(svg, "<text x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" dominant-baseline=\"hanging\">destino</text>", 0.0, y, color);
}

static void linhaPercursoInacessivel(FILE* svg, double x1, double y1, double x2, double y2, const char* color){
    fprintf(svg, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"%s\" stroke-width=\"4\" stroke-dasharray=\"4,6\" stroke-linecap=\"butt\" />", x1, y1, x2, y2, color);
}

static void catacSvg(FILE* svg, double x, double y, double w, double h, const char* cstrk, const char* cfill, double opacity){
    fprintf(svg, "<rect width=\"%.1lf\" height=\"%.1lf\" x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%.1f\" opacity=\"%lf\" />\n", w, h, x, y, cfill, cstrk, 2.0, opacity);
}

static void printQuadraToSVGvoid(Info info, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* extra){
    printQuadraToSVG(info, (FILE*)extra);
}

// FUNCAO PRINCIPAL
//////////////////////////////////////////////////////////////////////////////////////////////////

void processQryFile(Graph grafo, Quadras quadras, const char* path, FILE* svg, const char* txtPath){
    // Checa se o caminho dado contam a extensao .qry
    if(strstr(path, ".qry") == NULL){
        printf("\n- processQryFile() -> path: \"%s\" nao e' um arquivo .qry -", path);
        return;
    }
    // Checa se o caminho dado contam a extensao .svg
    if(svg == NULL){
        printf("\n- processQryFile() -> svg: Arquivo nulo. -");
        return;
    }
    // Checa se o caminho dado contam a extensao .txt
    if(strstr(txtPath, ".txt") == NULL){
        printf("\n- processQryFile() -> txtPath: \"%s\" nao e' um arquivo .txt -", txtPath);
        return;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("Lendo arquivo: %s\n", path);

    // Abre o arquivo texto em modo de escrita
    FILE* fTXT = fopen(txtPath, "w");
    printf("Escrevendo arquivo: %s\n", txtPath);

    // Abre um arquivo de output para as opecoes serem passadas ao .svg depois das quadras (overlay)
    FILE* queueQry = fopen("./queue.txt", "w");
    
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
        return;
    }

    percurso->origem = NULL;
    percurso->destino = NULL;
    percurso->caminhoCurto = NULL;
    percurso->caminhoRapido = NULL;

    bool gerarPercurso = false;

    // Itera sobre as linhas do arquivo .qry
    while(fscanf(fEntrada, "%s", op) > 0){
        if(strcmp(op, "@o?") == 0){
            // Pega [cep, face, num] da operacao de origem.
            fscanf(fEntrada, "%s %c %d\n", cep, &face, &num);

            percurso->origem = (Ponto*)malloc(sizeof(Ponto));
            if(checkAllocation(percurso->origem, "Ponto de origem do percurso.")){
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return;
            }

            Quadra quadra = getQuadraByID(quadras, cep);
            Coords coord = calculateCoordsByQuadraFaceNum(quadra, face, num);

            NodeST nodeSt = getClosestNodeSTrp(grafoSTreap, coord.x, coord.y);
            Info infoNode = getInfoSTrp(grafoSTreap, nodeSt, NULL, NULL, NULL, NULL, NULL, NULL);
            
            Node closestNode = *(Node*)infoNode;

            Info nodeInfo = getNodeInfo(grafo, closestNode);
            double x = getVerticeViaX(nodeInfo);
            double y = getVerticeViaY(nodeInfo);

            // Registra as informacoes do node encontrado na origem do percurso.
            percurso->origem->node = closestNode;
            percurso->origem->coords = (Coords){x, y};
            percurso->origem->face = face;
            percurso->origem->num = num;

            fprintf(fTXT, "\n [O] Ponto de origem adiquirido: x = %.1f, y = %.1f\n", x, y);
            linhaOrigemSvg(queueQry, x, y, "#008cffff");
        }
        else if(strcmp(op, "catac") == 0){
            // Pega [x, y, w, h] da operacao de cataclisma.
            fscanf(fEntrada, "%lf %lf %lf %lf\n", &x, &y, &w, &h);

            fprintf(fTXT, "\n [!] CATAC: x = %.1f, y = %.1f, width = %.1f, height = %.1f\n\n", x, y, w, h);

            Lista lista = criaLista();

            // Pega a lista das quadras dentro da regiao [x, y, w, h].
            getQuadrasRegion(quadras, x, y, w, h, lista);
            percorrerLista(lista, removerQuadraLista, &(ResourcesRemoverQuadra){quadras, fTXT});

            limparLista(lista, NULL, NULL);

            fprintf(fTXT, "\n");

            // Pega a lista dos vertices dentro da regiao [x, y, w, h].
            getNodeRegiaoSTrp(grafoSTreap, x, y, w, h, lista);
            percorrerLista(lista, removerArestasListaeSTreap, &(ResourcesEdgeRem){grafo, grafoSTreap, fTXT});

            limparLista(lista, NULL, NULL);

            catacSvg(queueQry, x, y, w, h, "#AA0044","#AB37C8", 0.5f);
        }
        else if(strcmp(op, "pnt") == 0){
            // Pega [cep, cfill, cstrk] da operacao de paint.
            fscanf(fEntrada, "%s %s %s\n", cep, cfill, cstrk);

            // Pega a quadra associada ao cep
            Quadra quadra = getQuadraByID(quadras, cep);

            fprintf(fTXT, "\n [PNT] Modificacao da quadra | CEP = %s\n", getQuadraID(quadra));
            fprintf(fTXT, "\n <cfill> Cor de preenchimento | %-20s -> %-10s", getQuadraCFill(quadra), cfill);
            fprintf(fTXT, "\n <cstrk> Cor de borda | %-20s -> %-10s\n", getQuadraCStrk(quadra), cstrk);

            // Muda as propriedades da quadra
            setQuadraCFill(quadra, cfill);
            setQuadraCStrk(quadra, cstrk);
        }
        else if(strcmp(op, "blq") == 0){
            // Pega [nome, sentido, x, y, w, h] da operacao de bloqueio.
            fscanf(fEntrada, "%s %s %lf %lf %lf %lf\n", nome, sentido, &x, &y, &w, &h);
        
            fprintf(fTXT, "\n [BLQ] Bloqueio de quadras | Nome = %s | Sentido = %s | x = %.1f, y = %.1f, w = %.1f, h = %.1f\n\n", nome, sentido, x, y, w, h);

            Lista lista = criaLista();

            // Pega os nodes dentro da regiao [x, y, w, h].
            getNodeRegiaoSTrp(grafoSTreap, x, y, w, h, lista);

            // Inicializa a estrutura de bloqueios a ser armazenada na tabela Hash.
            BloqueioStr* listaBlq = (BloqueioStr*)malloc(sizeof(BloqueioStr));
            if(checkAllocation(listaBlq, "Estrutura de bloqueios.")){
                if(percurso->origem) free(percurso->origem);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return;
            }

            listaBlq->nome = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
            if(checkAllocation(listaBlq->nome, "String de sentido do bloqueio.")){
                if(percurso->origem) free(percurso->origem);
                free(listaBlq);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return;
            }
            strcpy(listaBlq->nome, nome);

            listaBlq->lista = criaLista();

            // Percorre a lista dos nodes encontrados, bloqueando as arestas que estao no sentido fornecido.
            percorrerLista(lista, bloquearNodeSentido, &(ResourcesSentido){grafo, grafoSTreap, listaBlq->lista, sentido, fTXT});

            // Insere a estrutura BloqueioStr (Lista lista, char* sentido) na tabela Hash para busca do comando [rbl].
            inserirHash(tabelaHash, listaBlq->nome, listaBlq);

            limparLista(lista, NULL, NULL);
        }
        else if(strcmp(op, "rbl") == 0){
            // Pega [nome] da operacao de remocao de bloqueio.
            fscanf(fEntrada, "%s\n", nome);
            
            // Busca a estrutura de Bloqueio associada ao nome na tabela Hash.
            BloqueioStr* blqs = (BloqueioStr*)getHashValue(tabelaHash, nome);

            fprintf(fTXT, "\n [RBL] Desbloqueio de quadras | Nome = %s\n\n", blqs->nome);

            percorrerLista(blqs->lista, desbloquearSentido, &(RousourcesRBL){grafo, fTXT});
        }
        else if(strcmp(op, "b") == 0){
            // Pega [x, y, fator] da operacao de boost(?)
            fscanf(fEntrada, "%lf %lf %lf\n", &x, &y, &fator);

            // Pega o node mais pro'ximo das coordenadas.
            NodeST nodeSt = getClosestNodeSTrp(grafoSTreap, x, y);
            Node* nodep = (Node*)getInfoSTrp(NULL, nodeSt, NULL, NULL, NULL, NULL, NULL, NULL);

            // Comeca o percurso em largura do node.
            bfs(grafo, *nodep, aumentarVmArestas, &fator);
        }
        else if(strcmp(op, "p?") == 0){
            // Pega [cep, face, num, cmc, cmr] da operacao de percurso.
            fscanf(fEntrada, "%s %c %d %s %s\n", cep, &face, &num, cmc, cmr);

            if(percurso->origem == NULL){
                printf("\n- processQryFile() -> ponto de origem nao definido antes da operacao 'p?'. -");
                if(percurso->origem) free(percurso->origem);
                destroiHash(tabelaHash, freeBloqueio, NULL);
                free(percurso);
                return;
            }

            percurso->destino = (Ponto*)malloc(sizeof(Ponto));
            if(checkAllocation(percurso->destino, "Ponto de destino do percurso.")){
                if(percurso->origem) free(percurso->origem);
                free(percurso);
                return;
            }

            Quadra quadra = getQuadraByID(quadras, cep);
            Coords coord = calculateCoordsByQuadraFaceNum(quadra, face, num);

            NodeST nodeSt = getClosestNodeSTrp(grafoSTreap, coord.x, coord.y);
            Info infoNode = getInfoSTrp(grafoSTreap, nodeSt, NULL, NULL, NULL, NULL, NULL, NULL);
            
            Node closestNode = *(Node*)infoNode;

            Info nodeInfo = getNodeInfo(grafo, closestNode);
            double x = getVerticeViaX(nodeInfo);
            double y = getVerticeViaY(nodeInfo);

            // Registra as informacoes do node encontrado no destino do percurso.
            percurso->destino->node = closestNode;
            percurso->destino->coords = (Coords){x, y};
            percurso->destino->face = face;
            percurso->destino->num = num;

            strcpy(percurso->cmc, cmc);
            strcpy(percurso->cmr, cmr);

            fprintf(fTXT, "\n [P] Ponto de destino adiquirido: x = %.1f, y = %.1f\n", x, y);
            linhaDestinoSvg(queueQry, x, y, "#ff0000ff");

            gerarPercurso = true;
        }
        
        fprintf(fTXT, "\n==============================================================\n");

        if(percurso->destino != NULL && percurso->origem != NULL && gerarPercurso == true){
            fprintf(fTXT, "\n [V] Calculando rota entre os pontos de origem e destino.\n");
            
            // Calcula o caminho mais curto e o caminho mais ra'pido com Dijkstra (implementacao no final de digraph.c).
            // Usa a variável de comprimento da aresta para o caminho mais curto (getArestaCMPVoid).
            percurso->caminhoCurto = getShortestPath(grafo, percurso->origem->node, percurso->destino->node, getArestaCMPVoid);
            // Usa a variável de velocidade media da aresta para o caminho mais curto (getArestaVMVoid).
            percurso->caminhoRapido = getShortestPath(grafo, percurso->origem->node, percurso->destino->node, getArestaVMVoid);

            // Caso algum dos caminhos for nulo, aborta o retorno do percurso.
            if(percurso->caminhoCurto == NULL || percurso->caminhoRapido == NULL){
                fprintf(fTXT, "\n <!> Nao existe um caminho do ponto de origem ate o ponto de destino (%d -> %d).\n", percurso->origem->node, percurso->destino->node);
                fprintf(fTXT, "\n <o> Origem  | CEP = %-20s | (x, y) = (%4.1f, %4.1f)", getNodeName(grafo, percurso->origem->node), percurso->origem->coords.x, percurso->origem->coords.y);
                fprintf(fTXT, "\n <p> Destino | CEP = %-20s | (x, y) = (%4.1f, %4.1f)", getNodeName(grafo, percurso->destino->node), percurso->destino->coords.x, percurso->destino->coords.y);
                
                linhaPercursoInacessivel(svg, percurso->destino->coords.x, percurso->destino->coords.y, percurso->origem->coords.x, percurso->origem->coords.y, "#ff0000ff");
            }
            else{
                Lista listaRapido = getDijkstraList(percurso->caminhoRapido);
                Lista listaCurto = getDijkstraList(percurso->caminhoCurto);
                
                descreverCaminhoTXT(fTXT, "Rapido", listaRapido, grafo, percurso->origem->node, percurso->destino->node);
                descreverCaminhoTXT(fTXT, "Curto", listaCurto, grafo, percurso->origem->node, percurso->destino->node);
                
                animatePercurso(grafo, listaRapido, percurso->cmr, queueQry);
                animatePercurso(grafo, listaCurto, percurso->cmc, queueQry);
            }

            if(percurso->caminhoCurto != NULL) freeCaminho(percurso->caminhoCurto);
            if(percurso->caminhoRapido != NULL) freeCaminho(percurso->caminhoRapido);

            percurso->caminhoCurto = NULL;
            percurso->caminhoRapido = NULL;

            gerarPercurso = false;
        }
    }

    free(percurso->destino);
    free(percurso->origem);
    free(percurso);

    killSTrp(grafoSTreap, freeReg, NULL);
    destroiHash(tabelaHash, freeBloqueio, NULL);

    /////////////////////////////////////////

    fclose(queueQry);

    percorrerQuadras(quadras, printQuadraToSVGvoid, svg);
    copyFile(svg, "./queue.txt");

    remove("./queue.txt");

    // Fecha todos os arquivos
    fclose(fEntrada);
    fclose(fTXT);
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

void freePercurso(Percurso percurso, freeFunc freeCaminhoFunc, void* extra){
    PercursoStr* p = (PercursoStr*)percurso;
    
    if(freeCaminhoFunc != NULL){
        freeCaminhoFunc(p->caminhoCurto, extra);
        freeCaminhoFunc(p->caminhoRapido, extra);
    }

    free(p->origem);
    free(p->destino);
    free(p);
}
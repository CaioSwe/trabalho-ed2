#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "via.h"

typedef struct VerticeViaStr{
    double x, y;
}VerticeViaStr;

typedef struct ArestaViaStr{
    char* ldir;
    char* lesq;

    double cmp;
    double vm;

    bool habilitada;
} ArestaViaStr;

Graph processViaFile(const char* path){
    // Checa se o caminho dado contém a extensão .via
    if(strstr(path, ".via") == NULL){
        printf("\n- processViaFile() -> path: \"%s\" não é um arquivo .via -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    // Variável de operação.
    char op;

    // Variáveis de ID, posição (x, y) dos vértices, respectivamente.
    char id[256];
    double x, y;

    // Variáveis de nome, Cep (direito e esquerdo), comprimento, velocidade média das arestas, respectivamente.
    char nome[256];
    char ldir[256];
    char lesq[256];
    double cmp;
    double vm;

    char i[256];
    char j[256];

    // Lê a primeira linha do arquivo com o número de vértices do grafo.
    int nVert;
    fscanf(fEntrada, "%d", &nVert);

    // Grafo direcionado das formas lidas no arquivo .via
    Graph g = createGraph(nVert);

    // Itera sobre as linhas do arquivo .via
    while(fscanf(fEntrada, " %c", &op) == 1){
        if(op == 'v'){
            fscanf(fEntrada, "%s %lf %lf", id, &x, &y);

            VerticeViaStr* VV = (VerticeViaStr*)malloc(sizeof(VerticeViaStr));
            if(checkAllocation(VV, "Vertice-Via.")){
                killDG(g, freeReg, freeArestaVia);
                return NULL;
            }

            VV->x = x;
            VV->y = y;

            addNode(g, id, VV);
        }
        else if(op == 'e'){
            // Cria uma aresta [origem(i, j), vizinhos(ldir, lesq), comprimento, velocidade media e nome].
            fscanf(fEntrada, "%s %s %s %s %lf %lf %s", i, j, ldir, lesq, &cmp, &vm, nome);

            Node origem = getNode(g, i);
            Node destino = getNode(g, j);

            if(origem == destino && origem == -1){
                printf("\n- processViaFile() -> ID: %s ou %s ... Não encontrado. -", i, j);
                continue;
            }

            ArestaViaStr* via = (ArestaViaStr*)malloc(sizeof(ArestaViaStr));
            if(checkAllocation(via, "Aresta-Via.")){
                killDG(g, freeReg, freeArestaVia);
                return NULL;
            }

            via->ldir = (char*)malloc(sizeof(char) * strlen(ldir) + 1);
            if(checkAllocation(via->ldir, "Lado direito da aresta-via.")){
                free(via);
                killDG(g, freeReg, freeArestaVia);
                return NULL;
            }
            strcpy(via->ldir, ldir);

            via->lesq = (char*)malloc(sizeof(char) * strlen(lesq) + 1);
            if(checkAllocation(via->lesq, "Lado esquerdo da aresta-via.")){
                free(via->ldir);
                free(via);
                killDG(g, freeReg, freeArestaVia);
                return NULL;
            }
            strcpy(via->lesq, lesq);

            via->cmp = cmp;
            via->vm = vm;

            // Via e' habilitada por padrao em sua criacao.
            via->habilitada = true;

            Edge newEdge = addEdge(g, origem, destino, via);
            if(newEdge == NULL){
                printf("\n- processViaFile() -> Erro na criacao de nova aresta. -");
                killDG(g, freeReg, freeArestaVia);
                return NULL;
            }
        }
    }

    // Fecha o arquivo de entrada
    fclose(fEntrada);

    printf("\nNumero de Vertices lidos: %d\n", getTotalNodes(g));
    printf("Numero de Arestas lidas: %d\n", getTotalEdges(g));

    // Retorna o grafo inicializado com os vertices e arestas do .via
    return g;
}

// FUNÇÕES SET

void blockVia(ArestaVia av){
    ((ArestaViaStr*)av)->habilitada = false;
}

void unblockVia(ArestaVia av){
    ((ArestaViaStr*)av)->habilitada = true;
}

void setArestaVM(ArestaVia av, double vm){
    ((ArestaViaStr*)av)->vm = vm;
}

// FUNÇÕES GET

double getVerticeViaX(VerticeVia vv){
    return ((VerticeViaStr*)vv)->x;
}

double getVerticeViaY(VerticeVia vv){
    return ((VerticeViaStr*)vv)->y;
}

double getArestaVM(ArestaVia av){
    return ((ArestaViaStr*)av)->vm;
}

double getArestaCMP(ArestaVia av){
    return ((ArestaViaStr*)av)->cmp;
}

bool isArestaEnabled(ArestaVia av){
    return ((ArestaViaStr*)av)->habilitada;
}

// FREE

void freeArestaVia(ArestaVia av, void* extra){
    ArestaViaStr* arestaViav = (ArestaViaStr*)av;
    
    free(arestaViav->ldir);
    free(arestaViav->lesq);

    free(arestaViav);
}

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
    while (fscanf(fEntrada, " %c", &op) == 1){
        if(op == 'v'){
            fscanf(fEntrada, "%s %lf %lf", id, &x, &y);

            VerticeViaStr* VV = (VerticeViaStr*)malloc(sizeof(VerticeViaStr));
            VV->x = x;
            VV->y = y;

            addNode(g, id, VV);
        }
        else if(op == 'e'){
            fscanf(fEntrada, "%s %s %s %s %lf %lf %s", i, j, ldir, lesq, &cmp, &vm, nome);

            Node origem = getNode(g, i);
            Node destino = getNode(g, j);

            if(origem == destino && origem == -1){
                printf("\n- processViaFile() -> ID: %s ou %s ... Não encontrado. -", i, j);
                continue;
            }

            ArestaViaStr* via = (ArestaViaStr*)malloc(sizeof(ArestaViaStr));

            via->ldir = (char*)malloc(sizeof(char) * strlen(ldir) + 1);
            strcpy(via->ldir, ldir);

            via->lesq = (char*)malloc(sizeof(char) * strlen(lesq) + 1);
            strcpy(via->lesq, lesq);

            via->cmp = cmp;
            via->vm = vm;

            addEdge(g, origem, destino, via);
        }
    }

    printf("\n");

    // Fecha o arquivo de entrada
    fclose(fEntrada);

    // Retorna o grafo inicializado com os vertices e arestas do .via
    return g;
}

// FUNÇÕES GET

double getVerticeViaX(const void* VV){
    return ((VerticeViaStr*)VV)->x;
}

double getVerticeViaY(const void* VV){
    return ((VerticeViaStr*)VV)->y;
}
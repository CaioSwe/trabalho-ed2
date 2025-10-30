#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "geo.h"

typedef struct Ponto{
    Node node;
    char face;
    int num;
}Ponto;

typedef struct Percurso{
    Ponto* origem;
    Ponto* destino;

    char cmc[256];
    char cmr[256];
}Percurso;

static void removerQuadras(Item item, void* extra){
    // desabillitarArestas(item);
    // ou alguma coisa assim
}

static void bloquearSentido(Item item, void* extra){
    char* sentido = (char*)extra;
    // bloquearArestas(item, sentido);
    // ou alguma coisa assim
}

Lista processQryFile(Graph grafo, Quadras quadras, STreap arvore, const char* path){
    // Checa se o caminho dado contém a extensão .qry
    if(strstr(path, ".qry") == NULL){
        printf("\n- processQryFile() -> path: \"%s\" não é um arquivo .qry -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    // Variáveis de operação, cep, face, numero, quadrado [x, y, width, height], cor de preenchimento, cor de borda, nome, sentido, fator escalar, cor do caminho curto e rápido, respectivamente.
    char op[256];
    
    char cep[256];
    char face;
    int num;
    
    double x, y, w, h;

    char cfill[256];
    char cstrk[256];

    char nome[256];
    char sentido[2];

    double fator;

    char cmc[256];
    char cmr[256];

    // Cria uma lista para as operações [catac, blq]
    Lista lista = criaLista();

    // Cria uma lista com os bloqueios feitos.
    Lista bloqueios = criaLista();

    Percurso* percurso = (Percurso*)malloc(sizeof(Percurso));

    percurso->origem = NULL;
    percurso->destino = NULL;

    // Itera sobre as linhas do arquivo .qry
    while(fscanf(fEntrada, "%s", op) > 0){
        if(strcmp(op, "@o?") == 0){
            // Pega [cep, face, num] da operação de origem.
            fscanf(fEntrada, "%s %s %d\n", cep, face, &num);

            // Pega o node com o cep registrado no grafo.
            Node node = getNode(grafo, cep);

            percurso->origem = (Ponto*)malloc(sizeof(Ponto));

            // Registra as informações do node encontrado na origem do percurso.
            percurso->origem->node = node;
            percurso->origem->face = face;
            percurso->origem->num = num;
        }
        else if(strcmp(op, "catac") == 0){
            // Pega [x, y, w, h]
            fscanf(fEntrada, "%lf %lf %lf %lf\n", &x, &y, &w, &h);

            // Pega a lista das quadras dentro da região [x, y, w, h].
            getNodeRegiaoSTrp(arvore, x, y, w, h, lista);
            // Percorre a lista das quadras da região, desabilitando-as.
            percorrerLista(lista, removerQuadras, NULL);

            limparLista(lista, false);
        }
        else if(strcmp(op, "pnt") == 0){
            // Pega [cep, cfill, cstrk]
            fscanf(fEntrada, "%s %s %s\n", cep, cfill, cstrk);

            // Pega a quadra associada ao cep
            Quadra quadra = getQuadraByID(quadras, cep);

            // Muda as propriedades da quadra
            setQuadraCFill(quadra, cfill);
            setQuadraCStrk(quadra, cstrk);
        }
        else if(strcmp(op, "blq") == 0){
            // Pega [nome, sentido, x, y, w, h]
            fscanf(fEntrada, "%s %s %lf %lf %lf %lf\n", nome, sentido, &x, &y, &w, &h);
        
            // Pega as quadras dentro da região [x, y, w, h].
            getNodeRegiaoSTrp(arvore, x, y, w, h, lista);

            // Percorre a lista das quadras encontradas, bloqueando-as no sentido fornecido.
            percorrerLista(lista, bloquearSentido, sentido);

            limparLista(lista, false);
        }
        else if(strcmp(op, "rbl") == 0){
            // Pega [nome]
            fscanf(fEntrada, "%s\n", nome);
        
            // Desbloqueia as quadras com a operação blq de mesmo nome.
            // .
        }
        else if(strcmp(op, "b") == 0){
            // Pega [x, y, fator]
            fscanf(fEntrada, "%lf %lf %s\n", &x, &y, fator);
        
            // Pega o node mais próximo das coordenadas.
            // getClosestNode(graph, x, y);

            // Começa o percurso em largura do node.
            // bfs(g, node, discoverNode, extra)
        }
        else if(strcmp(op, "p?") == 0){
            // Pega [cep, face, num, cmc, cmr]
            fscanf(fEntrada, "%s %c %d %s %s\n", cep, &face, &num, cmc, cmr);

            if(percurso->origem == NULL){
                printf("\n- processQryFile() -> ponto de origem não definido antes da operação \"(p?)\". -");
                continue;
            }

            percurso->destino = (Ponto*)malloc(sizeof(Ponto));

            percurso->destino->node = getNode(grafo, cep);
            percurso->destino->face = face;
            percurso->destino->num = num;

            strcpy(percurso->cmc, cmc);
            strcpy(percurso->cmr, cmr);

            // continuar (dijkstra)
        }
    }

    printf("\n");

    // Fecha o arquivo de entrada
    fclose(fEntrada);
    
    return criaLista();
}
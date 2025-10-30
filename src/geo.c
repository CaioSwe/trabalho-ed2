#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"

typedef struct QuadraStr{
    char* id; // Associação usada na tabela hash

    double x, y;
    double width, height;

    char* sw;
    char* cfill;
    char* cstrk;
} QuadraStr;

typedef struct QuadrasStr{
    int nQuadras;

    Hash tabelaHash;
    QuadraStr* quadras;
} QuadrasStr;

static void inserirQuadrasHash(Item item, void* estrutura){
    QuadraStr* quadra = (QuadraStr*)item;
    QuadrasStr* quadras = (QuadrasStr*)estrutura;

    inserirHash(quadras->tabelaHash, quadra->id, quadras->nQuadras);

    quadras->quadras[quadras->nQuadras].id = quadra->id;
    quadras->quadras[quadras->nQuadras].x = quadra->x;
    quadras->quadras[quadras->nQuadras].y = quadra->y;
    quadras->quadras[quadras->nQuadras].width = quadra->width;
    quadras->quadras[quadras->nQuadras].height = quadra->height;
    quadras->quadras[quadras->nQuadras].sw = quadra->sw;
    quadras->quadras[quadras->nQuadras].cfill = quadra->cfill;
    quadras->quadras[quadras->nQuadras].cstrk = quadra->cstrk;
    
    quadras->nQuadras += 1;
}

void percorrerQuadras(Quadras quadras, runThroughQuadras runFunc, void* extra){
    QuadrasStr* quadrasStr = (QuadrasStr*)quadras;

    for(int i = 0; i < quadrasStr->nQuadras; i++) runFunc((Quadra)&quadrasStr->quadras[i], extra);
}

Quadras processGeoFile(const char* path){
    // Checa se o caminho dado contém a extensão .geo
    if(strstr(path, ".geo") == NULL){
        printf("\n- processGeoFile() -> path: \"%s\" não é um arquivo .geo -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    Lista lista = criaLista();

    // Variáveis de operação, ID da quadra e posição, respectivamente.
    char op[256];
    char id[256];
    double x, y, width, height;

    // Variáveis de espessura da borda, cor de preenchimento e borda, respectivamente.
    char sw[256];
    char cfill[256];
    char cstrk[256];

    // Itera sobre as linhas do arquivo .geo
    while(fscanf(fEntrada, "%s", op) > 0){
        if(strcmp(op, "cq") == 0){
            fscanf(fEntrada, "%s %s %s\n", sw, cfill, cstrk);
        }
        else if(strcmp(op, "q") == 0){
            fscanf(fEntrada, "%s %lf %lf %lf %lf\n", id, &x, &y, &width, &height);

            QuadraStr* quadra = malloc(sizeof(QuadraStr));

            quadra->id = malloc(sizeof(char) * strlen(id) + 1);
            strcpy(quadra->id, id);

            quadra->x = x;
            quadra->y = y;
            quadra->width = width;
            quadra->height = height;

            quadra->cfill = malloc(sizeof(char) * strlen(cfill) + 1);
            strcpy(quadra->cfill, cfill);
            quadra->cstrk = malloc(sizeof(char) * strlen(cstrk) + 1);
            strcpy(quadra->cstrk, cstrk);
            quadra->sw = malloc(sizeof(char) * strlen(sw) + 1);
            strcpy(quadra->sw, sw);

            inserirFim(lista, quadra);
        }
    }

    printf("\n");

    // Fecha o arquivo de entrada
    fclose(fEntrada);

    int nQuadras = listaTamanho(lista) + 1;

    printf("\nNumero de quadras lidas: %d\n", nQuadras - 1);

    // Cria uma estrutura de quadras
    QuadrasStr* quadras = (QuadrasStr*)malloc(sizeof(QuadrasStr));
    quadras->nQuadras = 0;
    quadras->tabelaHash = criaHash(nQuadras, true);

    quadras->quadras = (QuadraStr*)malloc(nQuadras * sizeof(QuadraStr));

    percorrerLista(lista, inserirQuadrasHash, quadras);

    // Retorna a estrutura Quadras das quadras lidas no .geo
    return quadras;
}

// FUNÇÕES GET

const char* getQuadraID(Quadra quadra){
    return ((QuadraStr*)quadra)->id;
}

double getQuadraX(Quadra quadra){
    return ((QuadraStr*)quadra)->x;
}

double getQuadraY(Quadra quadra){
    return ((QuadraStr*)quadra)->y;
}

double getQuadraWidth(Quadra quadra){
    return ((QuadraStr*)quadra)->width;
}

double getQuadraHeight(Quadra quadra){
    return ((QuadraStr*)quadra)->height;
}

const char* getQuadraCFill(Quadra quadra){
    return ((QuadraStr*)quadra)->cfill;
}

const char* getQuadraCStrk(Quadra quadra){
    return ((QuadraStr*)quadra)->cstrk;
}

const char* getQuadraSW(Quadra quadra){
    return ((QuadraStr*)quadra)->sw;
}

Quadra getQuadraByID(Quadras quadras, const char* id){
    QuadrasStr* qs = (QuadrasStr*)quadras;

    int index = (int)getHashValue(qs->tabelaHash, id);
    if(index == -1) return NULL;

    return &(qs->quadras[index]);
}

void setQuadraCFill(Quadra quadra, const char* cfill){
    QuadraStr* q = (QuadraStr*)quadra;

    free(q->cfill);
    q->cfill = malloc(sizeof(char) * (strlen(cfill) + 1));
    strcpy(q->cfill, cfill);
}

void setQuadraCStrk(Quadra quadra, const char* cstrk){
    QuadraStr* q = (QuadraStr*)quadra;

    free(q->cstrk);
    q->cstrk = malloc(sizeof(char) * (strlen(cstrk) + 1));
    strcpy(q->cstrk, cstrk);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"
#include "fileManager.h"

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

    int* nQuadrasp = (int*)malloc(sizeof(int));
    if(checkAllocation(nQuadrasp, "Ponteiro do enderenco de Quadra.")) return;

    *nQuadrasp = quadras->nQuadras;

    inserirHash(quadras->tabelaHash, quadra->id, nQuadrasp);

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

            QuadraStr* quadra = (QuadraStr*)malloc(sizeof(QuadraStr));
            if(checkAllocation(quadra, "Quadra.")){
                limparLista(lista, freeQuadra, NULL);
                return NULL;
            }

            quadra->id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
            if(checkAllocation(quadra->id, "String de id da quadra.")){
                free(quadra);
                limparLista(lista, freeQuadra, NULL);
                return NULL;
            }
            strcpy(quadra->id, id);

            quadra->x = x;
            quadra->y = y;
            quadra->width = width;
            quadra->height = height;

            quadra->cfill = (char*)malloc(sizeof(char) * (strlen(cfill) + 1));
            if(checkAllocation(quadra->cfill, "Cor de preenchimento da quadra.")){
                free(quadra->id);
                free(quadra);
                limparLista(lista, freeQuadra, NULL);
                return NULL;
            }
            strcpy(quadra->cfill, cfill);

            quadra->cstrk = (char*)malloc(sizeof(char) * (strlen(cstrk) + 1));
            if(checkAllocation(quadra->cstrk, "Cor da borda da quadra.")){
                free(quadra->cfill);
                free(quadra->id);
                free(quadra);
                limparLista(lista, freeQuadra, NULL);
                return NULL;
            }
            strcpy(quadra->cstrk, cstrk);

            quadra->sw = (char*)malloc(sizeof(char) * (strlen(sw) + 1));
            if(checkAllocation(quadra->sw, "Tamanho da borda da quadra.")){
                free(quadra->cstrk);
                free(quadra->cfill);
                free(quadra->id);
                free(quadra);
                limparLista(lista, freeQuadra, NULL);
                return NULL;
            }
            strcpy(quadra->sw, sw);

            inserirFim(lista, quadra);
        }
    }
    
    // Fecha o arquivo de entrada
    fclose(fEntrada);

    int nQuadras = listaTamanho(lista);

    printf("\nNumero de quadras lidas: %d\n", nQuadras);

    // Cria uma estrutura de quadras
    QuadrasStr* quadras = (QuadrasStr*)malloc(sizeof(QuadrasStr));
    if(checkAllocation(quadras, "Quadras.")){
        limparLista(lista, freeQuadra, NULL);
        return NULL;
    }

    quadras->nQuadras = 0;
    quadras->tabelaHash = criaHash(nQuadras, true);

    quadras->quadras = (QuadraStr*)malloc(nQuadras * sizeof(QuadraStr));
    if(checkAllocation(quadras, "Array de quadras da estrutura 'quadras'.")){
        destroiHash(quadras->tabelaHash, freeReg, NULL);
        free(quadras);
        limparLista(lista, freeQuadra, NULL);
        return NULL;
    }

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

    int* index = (int*)getHashValue(qs->tabelaHash, id);
    if(index == NULL) return NULL;

    return &(qs->quadras[*index]);
}

void setQuadraCFill(Quadra quadra, const char* cfill){
    QuadraStr* q = (QuadraStr*)quadra;

    free(q->cfill);
    q->cfill = malloc(sizeof(char) * (strlen(cfill) + 1));
    if(checkAllocation(q->cfill, "Cor de preenchimento da quadra (set).")) return;

    strcpy(q->cfill, cfill);
}

void setQuadraCStrk(Quadra quadra, const char* cstrk){
    QuadraStr* q = (QuadraStr*)quadra;

    free(q->cstrk);
    q->cstrk = malloc(sizeof(char) * (strlen(cstrk) + 1));
    if(checkAllocation(q->cstrk, "Cor de borda da quadra (set).")) return;

    strcpy(q->cstrk, cstrk);
}

void freeQuadra(Quadra quadra, void* extra){
    QuadraStr* q = (QuadraStr*)quadra;

    free(q->cfill);
    free(q->cstrk);
    free(q->sw);
    free(q->id);
    free(q);
}

void freeQuadras(Quadras quadras, void* extra){
    QuadrasStr* qs = (QuadrasStr*)quadras;

    for(int i = 0; i < qs->nQuadras; i++){
        freeQuadra(&qs->quadras[i], NULL);
    }
    free(qs->quadras);
    destroiHash(qs->tabelaHash, freeReg, NULL);
    
    free(qs);
}
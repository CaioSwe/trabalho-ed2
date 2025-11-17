#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"
#include "fileManager.h"

typedef struct QuadraStr{
    char* id; // Associação usada na tabela hash

    double x, y; // Chave usada na insercao da streap
    double width, height;

    char* sw;
    char* cfill;
    char* cstrk;
} QuadraStr;

typedef struct QuadrasStr{
    int maxQuadras;
    int nQuadras;

    Hash tabelaHash;
    STreap streap;
} QuadrasStr;

// Declaracao de escopo de funcoes.
static void freeQuadra(Quadra quadra, void* extra);

Quadras processGeoFile(const char* path){
    // Checa se o caminho dado contém a extensão .geo
    if(strstr(path, ".geo") == NULL){
        printf("\n- processGeoFile() -> path: \"%s\" não é um arquivo .geo -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    // Cria uma estrutura de quadras
    QuadrasStr* quadras = (QuadrasStr*)malloc(sizeof(QuadrasStr));
    if(checkAllocation(quadras, "Quadras.")) return NULL;

    quadras->nQuadras = 0;
    quadras->maxQuadras = 50;
    quadras->tabelaHash = criaHash(50, true, 0.75f);

    quadras->streap = createSTrp(0.0000001f);

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
                freeQuadras(quadras, NULL);
                return NULL;
            }

            quadra->id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
            if(checkAllocation(quadra->id, "String de id da quadra.")){
                free(quadra);
                freeQuadras(quadras, NULL);
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
                freeQuadras(quadras, NULL);
                return NULL;
            }
            strcpy(quadra->cfill, cfill);

            quadra->cstrk = (char*)malloc(sizeof(char) * (strlen(cstrk) + 1));
            if(checkAllocation(quadra->cstrk, "Cor da borda da quadra.")){
                free(quadra->cfill);
                free(quadra->id);
                free(quadra);
                freeQuadras(quadras, NULL);
                return NULL;
            }
            strcpy(quadra->cstrk, cstrk);

            quadra->sw = (char*)malloc(sizeof(char) * (strlen(sw) + 1));
            if(checkAllocation(quadra->sw, "Tamanho da borda da quadra.")){
                free(quadra->cstrk);
                free(quadra->cfill);
                free(quadra->id);
                free(quadra);
                freeQuadras(quadras, NULL);
                return NULL;
            }
            strcpy(quadra->sw, sw);

            inserirHash(quadras->tabelaHash, quadra->id, quadra);
            insertSTrp(quadras->streap, quadra->x, quadra->y, quadra);
            quadras->nQuadras += 1;
        }
    }
    
    // Fecha o arquivo de entrada
    fclose(fEntrada);

    printf("\nNumero de quadras lidas: %d\n", quadras->nQuadras);

    // Retorna a estrutura Quadras das quadras lidas no .geo
    return quadras;
}

void percorrerQuadras(Quadras quadras, FvisitaNo runFunc, void* extra){
    if(quadras == NULL){
        printf("\n - percorrerQuadras() -> Quadra passada e' nula. - ");
        return;
    }

    if(runFunc == NULL){
        printf("\n - percorrerQuadras() -> Funcao de percursao nula. - ");
        return;
    }

    QuadrasStr* qs = (QuadrasStr*)quadras;

    percursoLargura(qs->streap, runFunc, extra);
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
    return getHashValue(qs->tabelaHash, id);
}

static Item convertNodeToQuadra(Item item){
    return getInfoSTrp(NULL, item);
}

void getQuadrasRegion(Quadras quadras, double x, double y, double w, double h, Lista resultado){
    Lista nodeSTs = criaLista();
    getNodeRegiaoSTrp(((QuadrasStr*)quadras)->streap, x, y, w, h, nodeSTs);

    mapTo(nodeSTs, resultado, convertNodeToQuadra, sizeof(QuadraStr));
}

// FUNCOES SET

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

void removerQuadra(Quadras quadras, Quadra quadra){
    if(quadras == NULL || quadra == NULL){
        printf("\n - removerQuadra() -> Quadras ou quadra passada e' nula. - ");
        return;
    }
    
    QuadrasStr* qs = (QuadrasStr*)quadras;
    QuadraStr* q = (QuadraStr*)quadra;

    NodeST nodest = getNodeSTrp(qs->streap, q->x, q->y);
    deleteNodeSTrp(qs->streap, nodest);
    QuadraStr* quadraRem = removeHashValue(qs->tabelaHash, q->id);

    freeQuadra(quadraRem, NULL);
}

static void freeQuadra(Quadra quadra, void* extra){
    QuadraStr* q = (QuadraStr*)quadra;

    free(q->cfill);
    free(q->cstrk);
    free(q->sw);
    free(q->id);
    free(q);
}

void freeQuadras(Quadras quadras, void* extra){
    if(quadras == NULL){
        printf("\n - freeQuadras() -> Quadra passada e' nula.");
        return;
    }
    
    QuadrasStr* qs = (QuadrasStr*)quadras;
    
    killSTrp(qs->streap, freeQuadra, NULL);
    destroiHash(qs->tabelaHash, NULL, NULL);
    free(qs);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"

typedef struct QuadraStr{
    char* id;

    double x, y;
    double width, height;

    char* sw;
    char* cfill;
    char* cstrk;
} QuadraStr;

Lista processGeoFile(const char* path){
    // Checa se o caminho dado contém a extensão .geo
    if(strstr(path, ".geo") == NULL){
        printf("\n- processGeoFile() -> path: \"%s\" não é um arquivo .geo -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    // Variáveis de operação, ID da quadra e posição, respectivamente.
    char op[256];
    char id[256];
    double x, y, width, height;

    // Variáveis de espessura da borda, cor de preenchimento e borda, respectivamente.
    char sw[256];
    char cfill[256];
    char cstrk[256];

    // Lista genérica das formas lidas no arquivo .geo
    Lista formas = criaLista();

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

            inserirFim(formas, quadra);
        }
    }

    printf("\n");

    // Fecha o arquivo de entrada
    fclose(fEntrada);

    // Retorna a lista das formas lidas no .geo
    return formas;
}

// FUNÇÕES GET

double getQuadraX(const void* quadra){
    return ((QuadraStr*)quadra)->x;
}

double getQuadraY(const void* quadra){
    return ((QuadraStr*)quadra)->y;
}

double getQuadraWidth(const void* quadra){
    return ((QuadraStr*)quadra)->width;
}

double getQuadraHeight(const void* quadra){
    return ((QuadraStr*)quadra)->height;
}

const char* getQuadraCFill(const void* quadra){
    return ((QuadraStr*)quadra)->cfill;
}

const char* getQuadraCStrk(const void* quadra){
    return ((QuadraStr*)quadra)->cstrk;
};

const char* getQuadraSW(const void* quadra){
    return ((QuadraStr*)quadra)->sw;
}
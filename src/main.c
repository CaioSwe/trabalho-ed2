#include "fileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MeuGrafo.h"
#include "MinhaLista.h"

typedef enum Caminhos {
    ENTRADA,
    SAIDA,
    CAMINHO,
    QUERY
} caminhos;

typedef struct Info{
    char* sw;
    char* cfill;
    char* cstrk;
}Info;

typedef struct N{
    Rectangle rec;
    Info info;
}n;

void printToSVG(const void* item, const void* aux){
    Rectangle* rec = (Rectangle*)item;
    FILE* arquivo = (FILE*)aux;
    fprintf(arquivo, "<rect width=\"%.1f\" height=\"%.1f\" x=\"%.1f\" y=\"%.1f\" fill=\"blue\" />\n", rec->width, rec->height, rec->x, rec->y);
}

float maxX = 0;
float maxY = 0;

void getMax(const void* item){
    Rectangle* rec = (Rectangle*)item;

    if(rec->x + rec->width > maxX) maxX = rec->x + rec->width;
    if(rec->y + rec->height > maxY) maxY = rec->y + rec->height;
}

int main(int argc, char* argv[]){
    char* flags[4] = {"-e", "-o", "-f", "-q"};
    char** paths = (char**)malloc(sizeof(char*) * 4);

    for(int i = 0; i < 4; i++){
        paths[i] = (char*)malloc(sizeof(char));
        paths[i] = NULL;
    }

    // Pega todos os nomes e caminhos dos arquivos base e organiza na variável paths
    // paths[0] = entrada
    // paths[1] = saida
    // paths[2] = caminho (absoluto ou relativo)
    // paths[3] = query
    for(int i = 1; i < argc; i += 2){
        for(int j = 0; j < 4; j++){
            if(strcmp(argv[i], flags[j]) == 0){
                free(paths[j]);
                paths[j] = (char*)malloc(sizeof(char) * strlen(argv[i + 1]));
                strcpy(paths[j], (const char*)argv[i + 1]);
            }
        }
    }

    // Caso a entrada não for especificado, usa o diretório atual
    if(paths[ENTRADA] == NULL){
        free(paths[ENTRADA]);
        paths[ENTRADA] = (char*)malloc(sizeof(char) * strlen("./"));
        strcpy(paths[ENTRADA], "./");
    }

    printf("\nEntrada: %s", paths[ENTRADA]);
    printf("\nSaida: %s", paths[SAIDA]);
    printf("\nArquivo Geo: %s", paths[CAMINHO]);
    printf("\nQuery: %s", paths[QUERY]);

    // Coloca '/' caso não tenha no final da entrada -e
    if(paths[ENTRADA][strlen(paths[ENTRADA]) - 1] != '/'){
        strncat(paths[ENTRADA], "/", (size_t)(paths[ENTRADA] + 1));
    }

    printf("\n\n");

    // Coloca ".geo" no arquivo geo caso não tenha
    strcpy(paths[CAMINHO], changeExtension(paths[CAMINHO], ".geo"));
    
    
    // (2) Utilizando o .geo
    ////////////////////////////////////////


    // Concatena o caminho e a entrada para fEntradaPath
    const char* fEntradaPath = strcatcat(paths[ENTRADA], paths[CAMINHO]);

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(fEntradaPath, "r");
    printf("Lendo arquivo: %s\n", fEntradaPath);

    // Variáveis de operação, ID da quadra e posição, respectivamente.
    char op[256];
    char quadraID[256];
    Rectangle posQuadra;

    // Variáveis de espessura da borda, cor de preenchimento e borda, respectivamente.
    char* sw[256];
    char* cfill[256];
    char* cstrk[256];

    // Lista genérica das formas lidas no arquivo .geo
    Lista* formas = criaLista();

    while(fscanf(fEntrada, "%s", op) > 0){
        if(strcmp(op, "cq") == 0){
            scanf(fEntrada, "%s %s %s\n", sw, &cfill, &cstrk);
        }
        else if(strcmp(op, "q") == 0){
            fscanf(fEntrada, "%s %f %f %f %f\n", quadraID, &posQuadra.x, &posQuadra.y, &posQuadra.width, &posQuadra.height);
            
            Rectangle* quadra = (Rectangle*)malloc(sizeof(Rectangle));
            
            quadra->x = posQuadra.x;
            quadra->y = posQuadra.y;
            quadra->width = posQuadra.width;
            quadra->height = posQuadra.height;

            inserirFim(formas, quadra);
        }
    }

    printf("\n");

    fclose(fEntrada);

    // (3) Abre um arquivo .svg e coloca as formas adiquiridas
    /////////////////////////////////////////

    // Concatena o diretório de saída com o nome do arquivo .geo em formato .svg
    const char* fOutputPath = strcatcat(paths[SAIDA], changeExtension(paths[CAMINHO], ".svg"));
    
    // Abre o diretório em modo de escrita
    FILE* fSaida = fopen(fOutputPath, "w");
    printf("Escrevendo no arquivo: %s\n", fOutputPath);

    percorrerLista(formas, getMax);

    fprintf(fSaida, "<svg width=\"%.1f\" height=\"%.1f\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX, maxY);
    percorrerListaRel(formas, printToSVG, fSaida);
    fprintf(fSaida, "</svg>");

    fclose(fSaida);

    return 0;
}
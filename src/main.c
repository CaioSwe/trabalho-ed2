#include "fileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <raylib.h>
#include "lista.h"

typedef enum GEOs {
    ENTRADA,
    SAIDA,
    GEO,
    QUERY,
    VIA
} GEOs;

typedef struct Info{
    char* id;

    Rectangle rec;
    char* sw;
    char* cfill;
    char* cstrk;
}Info;

/////////////// GRAFO DE PLACEHOLDER

typedef struct Vertices{
    int id;
    double x, y;
} Vertices;

typedef struct Arestas{
    char* nome;
    
    char* ldir, lesq;

    double cmp;
    double vm;
}Arestas;

typedef struct Grafo{
    int nVertices;
    int nArestas;

    int maxVertices;
    
    Vertices* vertices;
    Arestas* arestas;

    Lista* adjacencia;
}Grafo;

///////////////

void printToSVG(const void* item, const void* aux){
    Info* info = (Info*)item;
    FILE* arquivo = (FILE*)aux;
    fprintf(arquivo, "<rect width=\"%.1f\" height=\"%.1f\" x=\"%.1f\" y=\"%.1f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" />\n", info->rec.width, info->rec.height, info->rec.x, info->rec.y, info->cfill, info->cstrk, info->sw);
}

float maxX = 0;
float maxY = 0;

void getMax(const void* item){
    Rectangle* rec = (Rectangle*)item;

    if(rec->x + rec->width > maxX) maxX = rec->x + rec->width;
    if(rec->y + rec->height > maxY) maxY = rec->y + rec->height;
}

int main(int argc, char* argv[]){
    char* flags[5] = {"-e", "-o", "-f", "-q", "-v"};
    char** paths = (char**)malloc(sizeof(char*) * 5);

    for(int i = 0; i < 5; i++){
        paths[i] = (char*)malloc(sizeof(char));
        paths[i] = NULL;
    }

    // Pega todos os nomes e GEOs dos arquivos base e organiza na variável paths
    // paths[0] = entrada (absoluto ou relativo) (opcional)
    // paths[1] = saida (absoluto ou relativo)
    // paths[2] = GEO
    // paths[3] = query (opcional)
    // paths[4] = VIA (opcional)
    for(int i = 1; i < argc; i += 2){
        for(int j = 0; j < 5; j++){
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

    // Caso o path de saída seja nulo (não fornecido), é escolhido o diretório corrente (./)
    if(!paths[SAIDA]){
        free(paths[SAIDA]);
        paths[SAIDA] = (char*)malloc(sizeof(char) * strlen("./"));
        strcpy(paths[SAIDA], "./");
    }
    
    printf("\nEntrada: %s", paths[ENTRADA]);
    printf("\nSaida: %s", paths[SAIDA]);
    printf("\nArquivo Geo: %s", paths[GEO]);
    printf("\nQuery: %s", paths[QUERY]);

    // Coloca '/' caso não tenha no final do path de entrada
    if(paths[ENTRADA][strlen(paths[ENTRADA]) - 1] != '/'){
        strncat(paths[ENTRADA], "/", (size_t)(paths[ENTRADA] + 1));
    }

    printf("\n\n");

    // Coloca ".geo" no arquivo geo caso não tenha
    strcpy(paths[GEO], changeExtension(paths[GEO], ".geo"));
    

    // (2) Utilizando o .geo
    ////////////////////////////////////////

    // Concatena o path de entrada e o nome do arquivo GEO para fEntradaPath
    const char* fEntradaPath = strcatcat(paths[ENTRADA], paths[GEO]);

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(fEntradaPath, "r");
    printf("Lendo arquivo: %s\n", fEntradaPath);

    // Variáveis de operação, ID da quadra e posição, respectivamente.
    char op[256];
    char quadraID[256];
    Rectangle posQuadra;

    // Variáveis de espessura da borda, cor de preenchimento e borda, respectivamente.
    char sw[256];
    char cfill[256];
    char cstrk[256];

    // Lista genérica das formas lidas no arquivo .geo
    Lista* formas = criaLista();

    while(fscanf(fEntrada, "%s", op) > 0){
        if(strcmp(op, "cq") == 0){
            fscanf(fEntrada, "%s %s %s\n", sw, cfill, cstrk);
        }
        else if(strcmp(op, "q") == 0){
            fscanf(fEntrada, "%s %f %f %f %f\n", quadraID, &posQuadra.x, &posQuadra.y, &posQuadra.width, &posQuadra.height);
            
            Info* info = (Info*)malloc(sizeof(Info));
            
            //info->id = quadraID;

            info->rec.x = posQuadra.x;
            info->rec.y = posQuadra.y;
            info->rec.width = posQuadra.width;
            info->rec.height = posQuadra.height;

            info->cfill = cfill;
            info->sw = sw;
            info->cstrk = cstrk;

            inserirFim(formas, info);
        }
    }

    printf("\n");

    fclose(fEntrada);

    // (3) Abre um arquivo .svg e coloca as formas adiquiridas
    /////////////////////////////////////////

    // Concatena o diretório de saída com o nome do arquivo .geo em formato .svg
    const char* fOutputPath = strcatcat(paths[SAIDA], changeExtension(paths[GEO], ".svg"));
    
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
#include "fileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lista.h"
#include "svg.h"
#include "geo.h"

typedef enum GEOs {
    ENTRADA,
    SAIDA,
    GEO,
    QUERY,
    VIA
} GEOs;

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

float maxX = 0;
float maxY = 0;

void getMax(const void* item){
    // Abusando um pouco da função de mudar extensão :)
    // A função atoi() converte uma string em um inteiro equivalente.
    float itemW = getQuadraX(item) + getQuadraWidth(item) + atoi(changeExtension(getQuadraSW(item), "")) + 1;
    float itemH = getQuadraY(item) + getQuadraHeight(item) + atoi(changeExtension(getQuadraSW(item), "")) + 1;

    if(itemW > maxX) maxX = itemW;
    if(itemH > maxY) maxY = itemH;
}

int main(int argc, char* argv[]){
    char* flags[5] = {"-e", "-o", "-f", "-q", "-v"};
    char** paths = (char**)malloc(sizeof(char*) * 5);

    for(int i = 0; i < 5; i++){
        paths[i] = (char*)malloc(sizeof(char));
        paths[i] = NULL;
    }

    /*
        Pega todos os nomes e GEOs dos arquivos base e organiza na variável paths
        paths[0] = entrada (opcional)
        paths[1] = saida (obrigatório)
        paths[2] = GEO (obrigatório)
        paths[3] = query (opcional)
        paths[4] = VIA (opcional)
    */
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
    if(!paths[SAIDA] || !paths[GEO]){
        printf("\n- main() -> Erro na passagem de parâmetros obrigatórios: arquivo .geo ou caminho de saída não especificado. -");
        return 1;
    }
    
    // Coloca '/' caso não tenha no final do path de entrada
    if(paths[ENTRADA][strlen(paths[ENTRADA]) - 1] != '/'){
        strncat(paths[ENTRADA], "/", (size_t)(paths[ENTRADA] + 1));
    }

    printf("\n\n");

    // Coloca ".geo" no arquivo geo caso não tenha
    strcpy(paths[GEO], changeExtension(paths[GEO], ".geo"));

    // Coloca ".via" no arquivo via caso ele exista e não tenha
    if(paths[VIA]) strcpy(paths[VIA], changeExtension(paths[GEO], ".via"));
    
    // Todos os parâmetros fornecidos:
    printf("\nEntrada: %s", paths[ENTRADA]);
    printf("\nSaida: %s", paths[SAIDA]);
    printf("\nArquivo Geo: %s", paths[GEO]);
    printf("\nQuery: %s", paths[QUERY]);
    printf("\nArquivo Via: %s", paths[VIA]);

    // (2) Lendo o arquivo .geo
    ////////////////////////////////////////

    // Concatena o path de entrada e o nome do arquivo GEO para fEntradaPath
    const char* fEntradaPath = strcatcat(paths[ENTRADA], paths[GEO]);

    Lista formas = processGeoFile(fEntradaPath);

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
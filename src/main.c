#include "fileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lista.h"
#include "files.h"
#include "geo.h"
#include "digraph.h"
#include "via.h"

typedef enum GEOs {
    ENTRADA,
    SAIDA,
    GEO,
    QUERY,
    VIA
} GEOs;

///////////////

float maxX = 0;
float maxY = 0;

void getMax(Quadra quadra, void* extra){
    // Abusando um pouco da função de mudar extensão :)
    // A função atoi() converte uma string em um inteiro equivalente.
    float itemW = getQuadraX(quadra) + getQuadraWidth(quadra) + atoi(changeExtension(getQuadraSW(quadra), "")) + 1;
    float itemH = getQuadraY(quadra) + getQuadraHeight(quadra) + atoi(changeExtension(getQuadraSW(quadra), "")) + 1;

    if(itemW > maxX) maxX = itemW;
    if(itemH > maxY) maxY = itemH;
}

int main(int argc, char* argv[]){
    char* flags[5] = {"-e", "-o", "-f", "-q", "-v"};
    char** paths = calloc(5, sizeof(char*));

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
                paths[j] = malloc(strlen(argv[i + 1]) + 1);
                strcpy(paths[j], argv[i + 1]);
            }
        }
    }

    // Caso a entrada não for especificado, usa o diretório atual
    if(paths[ENTRADA] == NULL){
        free(paths[ENTRADA]);
        paths[ENTRADA] = (char*)malloc(sizeof(char) * (strlen("./") + 1));
        strcpy(paths[ENTRADA], "./");
    }

    // Caso o path de saída seja nulo (não fornecido), é escolhido o diretório corrente (./)
    if(!paths[SAIDA] || !paths[GEO]){
        printf("\n- main() -> Erro na passagem de parâmetros obrigatórios: arquivo .geo ou caminho de saída não especificado. -");
        return 1;
    }
    
    // Coloca '/' caso não tenha no final do path de entrada
    if(paths[ENTRADA][strlen(paths[ENTRADA]) - 1] != '/'){
        size_t len = strlen(paths[ENTRADA]);
        paths[ENTRADA] = realloc(paths[ENTRADA], len + 2);
        strcat(paths[ENTRADA], "/");
    }

    printf("\n\n");

    // Coloca ".geo" no arquivo geo caso não tenha
    char* newPathGeo = (char*)changeExtension(paths[GEO], ".geo");
    free(paths[GEO]);
    paths[GEO] = newPathGeo;

    // Coloca ".via" no arquivo via caso ele exista e não tenha
    if (paths[VIA]) {
        char* newPathVia = (char*)changeExtension(paths[VIA], ".via");
        free(paths[VIA]);
        paths[VIA] = newPathVia;
    }

    
    // Todos os parâmetros fornecidos:
    printf("%-12s: %s\n", "Entrada", paths[ENTRADA]);
    printf("%-12s: %s\n", "Saida",   paths[SAIDA]);
    printf("%-12s: %s\n", "Arquivo Geo", paths[GEO]);
    printf("%-12s: %s\n", "Query",   paths[QUERY]);
    printf("%-12s: %s\n", "Arquivo Via", paths[VIA]);

    // (2) Lendo o arquivo .geo
    ////////////////////////////////////////

    // Concatena o path de entrada e o nome do arquivo GEO para fEntradaPath
    const char* fEntradaPath = strcatcat(paths[ENTRADA], paths[GEO]);

    Lista formas = processGeoFile(fEntradaPath);

    // (3) Lendo o arquivo .via (se tiver)
    /////////////////////////////////////////////////////////////////

    Lista vertices = NULL;

    if(paths[VIA]){
        // Concatena o path de entrada e o nome do arquivo VIA para fViaPAth
        const char* fViaPath = strcatcat(paths[ENTRADA], paths[VIA]);

        // Guarda as informações do arquivo .via em um grafo direcionado.
        Graph grafo;
        grafo = processViaFile(fViaPath);

        // Inicializa uma lista dos vertices do grafo G para visualização no SVG (opcional)
        vertices = criaLista();
        getAllVerticesInfo(grafo, vertices);

        // (3.1) Escreve a lista de adjacência de G em um TXT [v -> e]
        /////////////////////////////////////////

        // Concatena o diretório de saída com o nome do arquivo .via em formato .txt
        const char* fOutputViaPath = strcatcat(paths[SAIDA], changeExtension(paths[VIA], ".txt"));

        // Abre o diretório em modo de escrita
        FILE* fViaSaida = fopen(fOutputViaPath, "w");
        printf("Escrevendo no arquivo: %s\n", fOutputViaPath);

        // ARRUMAR PARA: bfs ou dfs -> percorrerGrafoRel(grafo, printToTXT, fViaSaida);

        fclose(fViaSaida);
    }

    // (4) Abre um arquivo .svg e coloca as formas adiquiridas do arquivo .geo
    /////////////////////////////////////////

    // Concatena o diretório de saída com o nome do arquivo .geo em formato .svg
    const char* fOutputPath = strcatcat(paths[SAIDA], changeExtension(paths[GEO], ".svg"));
    
    // Abre o diretório em modo de escrita
    FILE* fSaida = fopen(fOutputPath, "w");
    printf("Escrevendo no arquivo: %s\n", fOutputPath);

    percorrerQuadras(formas, getMax, NULL);

    fprintf(fSaida, "<svg width=\"%.1f\" height=\"%.1f\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX, maxY);
    percorrerQuadras(formas, printQuadrasToSVG, fSaida);
    // Tire de comentário caso queria visualizar os vertices do grafo no SVG produzido pelo arquivo .via
    // percorrerLista(vertices, printVerticesToSVG, fSaida);
    fprintf(fSaida, "</svg>");

    fclose(fSaida);

    printf("\nPrograma finalizado com sucesso!\n");

    return 0;
}
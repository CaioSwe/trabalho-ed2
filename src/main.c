#include "fileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lista.h"
#include "files.h"
#include "geo.h"
#include "digraph.h"
#include "via.h"
#include "qry.h"

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

void getMax(Quadra quadra, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* extra){
    // Abusando um pouco da função de mudar extensão :) (trunca SW [ex: 1.5] para [1])
    // A função atoi() converte uma string em um inteiro equivalente.
    float itemW = getQuadraX(quadra) + getQuadraWidth(quadra) + atoi(changeExtension(getQuadraSW(quadra), "")) + 1;
    float itemH = getQuadraY(quadra) + getQuadraHeight(quadra) + atoi(changeExtension(getQuadraSW(quadra), "")) + 1;

    if(itemW > maxX) maxX = itemW;
    if(itemH > maxY) maxY = itemH;
}

void printQuadraToSVGvoid(Info info, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* extra){
    printQuadraToSVG(info, (FILE*)extra);
}

int main(int argc, char* argv[]){
    // Geracao de prioridade aleatoria da STreap.
    srand((unsigned)time(NULL));

    char* flags[5] = {"-e", "-o", "-f", "-q", "-v"};
    char** paths = calloc(5, sizeof(char*));

    /*
        Pega todos os nomes e GEOs dos arquivos base e organiza na variável paths
        paths[0] = (-e) entrada      (opcional)
        paths[1] = (-o) saida        (obrigatório)
        paths[2] = (-f) GEO          (obrigatório)
        paths[3] = (-q) query        (opcional)
        paths[4] = (-v) VIA          (opcional)
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

    // Caso a entrada não for especificada, usa o diretório atual
    if(paths[ENTRADA] == NULL){
        free(paths[ENTRADA]);
        paths[ENTRADA] = (char*)malloc(sizeof(char) * (strlen("./") + 1));
        if(checkAllocation(paths[ENTRADA], "String do caminho de entrada.")){
            for(int i = 1; i < 5; i++) free(paths[i]);
            free(paths);
            return 1;
        }
        strcpy(paths[ENTRADA], "./");
    }

    // Caso o path de saída seja nulo (não fornecido), é escolhido o diretório corrente (./)
    if(!paths[SAIDA] || !paths[GEO]){
        printf("\n- main() -> Erro na passagem de parâmetros obrigatórios: arquivo .geo ou caminho de saída não especificado. -");
        return 1;
    }
    
    // Coloca '/' caso não tenha no final do path de entrada
    if(paths[ENTRADA][strlen(paths[ENTRADA]) - 1] != '/'){
        paths[ENTRADA] = realloc(paths[ENTRADA], strlen(paths[ENTRADA]) + 2);
        if(checkAllocation(paths[ENTRADA], "Realloc da string do caminho de entrada.")){
            for(int i = 1; i < 5; i++) free(paths[i]);
            free(paths);
            return 1;
        }
        strcat(paths[ENTRADA], "/");
    }

    // Coloca '/' caso não tenha no final do path de saida
    if(paths[SAIDA][strlen(paths[SAIDA]) - 1] != '/'){
        paths[SAIDA] = realloc(paths[SAIDA], strlen(paths[SAIDA]) + 2);
        if(checkAllocation(paths[SAIDA], "Realloc da string do caminho de saida.")){
            for(int i = 1; i < 5; i++) free(paths[i]);
            free(paths);
            return 1;
        }
        strcat(paths[SAIDA], "/");
    }

    printf("\n\n");

    // Coloca ".geo" no arquivo geo caso não tenha
    char* newPathGeo = (char*)changeExtension(paths[GEO], ".geo");
    free(paths[GEO]);
    paths[GEO] = newPathGeo;

    // Coloca ".via" no arquivo via caso ele exista e não tenha
    if(paths[VIA]){
        char* newPathVia = (char*)changeExtension(paths[VIA], ".via");
        free(paths[VIA]);
        paths[VIA] = newPathVia;
    }

    // Lista todos os parâmetros fornecidos:
    printf("%-12s: %s\n", "Entrada", paths[ENTRADA]);
    printf("%-12s: %s\n", "Saida", paths[SAIDA]);
    printf("%-12s: %s\n", "Arquivo Geo", paths[GEO]);
    printf("%-12s: %s\n", "Query", paths[QUERY]);
    printf("%-12s: %s\n", "Arquivo Via", paths[VIA]);

    // (2) Lendo o arquivo .geo
    ////////////////////////////////////////

    // Concatena o path de entrada e o nome do arquivo GEO para fEntradaPath.
    const char* fEntradaPath = strcatcat(paths[ENTRADA], paths[GEO]);

    // Guarda as informações do geo em um ponteiro para a memoria das quadras.
    Quadras formas = processGeoFile(fEntradaPath);

    // (2.1) Lendo o arquivo .via (se tiver)
    ////////////////////////////////////////////////

    Graph grafo = NULL;

    if(paths[VIA]){
        // Concatena o path de entrada e o nome do arquivo VIA para fViaPath.
        const char* fViaPath = strcatcat(paths[ENTRADA], paths[VIA]);
        
        // Guarda as informações do arquivo .via em um grafo direcionado.
        grafo = processViaFile(fViaPath);
    }

    // (2.2) Abre um arquivo .svg e coloca as formas adiquiridas do arquivo .geo
    ////////////////////////////////////////////////

    // Concatena o diretório de saída com o nome do arquivo .geo em formato .svg
    const char* fOutputPath = strcatcat(paths[SAIDA], changeExtension(paths[GEO], ".svg"));
    
    // Abre o diretório em modo de escrita
    FILE* fSaida = fopen(fOutputPath, "w");
    printf("\nEscrevendo no arquivo: %s\n", fOutputPath);

    // Percore todas as quadras para encontrar o maior X e Y (Dimensoes do SVG).
    percorrerQuadras(formas, getMax, NULL);

    // Abre e fecha o cabecalho do SVG, iterando sob as quadras.
    fprintf(fSaida, "<svg viewBox=\"-39.0 -39.0 %.1f %.1f\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX * 1.2f, maxY * 1.2f);
    percorrerQuadras(formas, printQuadraToSVGvoid, fSaida);
    fprintf(fSaida, "</svg>");

    // Fecha o diretório
    fclose(fSaida);

    // (3) Lendo o arquivo .qry (se tiver)
    ////////////////////////////////////////////////
    if(paths[QUERY]){
        // Concatena o path de entrada e o nome do arquivo QRY para fEntradaPath
        const char* fEntradaPathQry = strcatcat(paths[ENTRADA], paths[QUERY]);

        // Acha as ocorrencias de '/' no caminho query e substitui com '-'
        for(int i = 0; paths[QUERY][i] != '\0'; i++){
            if(paths[QUERY][i] == '/'){
                paths[QUERY][i] = '-';
                break;
            }
        }

        // Concatena o diretório de saída com o nome do arquivo .geo + .qry em formato .txt
        const char* fOutputPathQryTxt = changeExtension(strcatcat(paths[SAIDA], paths[QUERY]), ".txt");

        // Reproduz o caminho do .txt e converte para .svg
        const char* fOutputPathQry = changeExtension(fOutputPathQryTxt, ".svg");

        // Abre o caminho do output .svg em modo de escrita
        fSaida = fopen(fOutputPathQry, "w");
        printf("\nEscrevendo no arquivo: %s\n", fOutputPathQry);
        fprintf(fSaida, "<svg viewBox=\"-39.0 -39.0 %.1f %.1f\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX * 1.2f, maxY * 1.2f);

        processQryFile(grafo, formas, fEntradaPathQry, fSaida, fOutputPathQryTxt);
        
        fprintf(fSaida, "</svg>");
        fclose(fSaida);
    }

    // (3.1) Coloca as quadras no arquivo .dot
    ////////////////////////////////////////////////

    // Se o .qry existir, faz o caminho de saida .qry em .dot / caso contrário, usa a saída do .geo em .dot
    const char* fSaidaDot = paths[QUERY] ? strcatcat(changeExtension(fOutputPath, "-"), changeExtension(paths[QUERY], ".dot")) : changeExtension(fOutputPath, ".dot");
    printSTrp(getQuadrasSTrp(formas), fSaidaDot);

    // (4) SECAO DE LIBERACAO DE MEMORIA
    ////////////////////////////////////////////////

    // Paths
    for(int i = 1; i < 5; i++) free(paths[i]);
    free(paths);
    
    // Geo
    freeQuadras(formas, NULL);
    
    // Via
    if(grafo != NULL) killDG(grafo, freeReg, freeArestaVia);
    
    printf("\nProcessamento finalizado!\n");

    return 0;
}
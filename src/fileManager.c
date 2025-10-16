#include "fileManager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char* changeExtension(const char* path, const char* ext){
    // Procura a primeira ocorrência do caractere '.' na string path.
    char* d = strrchr(path, '.');

    // Aloca espaço necessário para ambos os casos de '.' estar presente ou não.
    char* newStr = (d != NULL ? (char*)malloc(sizeof(char) * (d - path + (int)strlen(ext) + 1)) : (char*)malloc(sizeof(char) * (strlen(path) + strlen(ext) + 1)));
    if(newStr == NULL){
        printf("\n- changeExtension() -> Erro na alocação de memória. -");
        return NULL;
    }

    // Caso d seja um caractere não nulo (achado), copia path para newStr até o caractere antes do '.'.
    if(d != NULL){
        strncpy(newStr, path, d - path);
        newStr[d - path] = '\0';
    }
    // Caso d seja nulo, copia todo o path para newStr.
    else strcpy(newStr, path);

    // Retorna newStr concatenado com a nova entensão ext.
    return (const char*)strcat(newStr, ext);
}

const char* trimString(const char* string, int n){
    // Adiquire o tamanho da string original.
    int stringSize = (int)strlen(string);

    // Caso o inteiro N for maior que o tamanho da string fornecida, imprime mensagem de aviso e retorna NULO.
    if(n > stringSize){
        printf("\n- trimString() -> Valor dado eh maior que a string dada: (%s) > (%d) -\n", stringSize, n);
        return NULL;
    }
    
    // Calcula o tamanho necessário para a nova string truncada.
    int sizeofResult = (stringSize - n + 1);

    // Dinâmicamente aloca espaço para a string.
    char* resultTrim = (char*)malloc(sizeof(char) * sizeofResult);
    if(resultTrim == NULL){
        printf("\n- trimString() -> Erro na alocação de memória da string.");
        return NULL;
    }

    // Copia o espaço de memória ocupado pela string original para a string truncada com o tamanho calculado.
    memcpy(resultTrim, string, sizeofResult - 1);
    // Substitui o último caratere pelo terminador '\0'.
    resultTrim[sizeofResult - 1] = '\0';

    // Retorna a string truncada.
    return (const char*)resultTrim;
}

const char* strcatcat(const char* cat1, const char* cat2){
    // Dinâmicamente aloca um espaço do tamanho da string1 + string2 + caractere nulo
    char* resultCat = (char*)malloc(sizeof(char) * (strlen(cat1) + strlen(cat2) + 1));
    if(resultCat == NULL){
        printf("\n- strcatcat() -> Erro na alocação de memória da string.");
        return NULL;
    }

    // Copia e concatena a string1 e string2 para resultCat.
    strcpy(resultCat, cat1);
    strcat(resultCat, cat2);

    // Retorna a string concatenada.
    return (const char*)resultCat;
}
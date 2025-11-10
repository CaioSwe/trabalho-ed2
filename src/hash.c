#include "hash.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct HashCel{
    char* chave;
    HashItem valor;

    struct HashCel* prox;
} HashCel;

typedef struct HashStr{
    int tam;
    int qPreenchida;
    double fPreenchimento;
    HashCel** balde;
} HashStr;

static bool isPrimo(int v){
    if(v < 2) return false;

    int raizV = sqrt(v);
    for(int i = 2; i <= raizV; i++){
        if(v % i == 0) return false;
    }
    return true;
}

static int proxPrimo(int v){
    v += (v % 2 == 0 ? 1 : 2);
    while(isPrimo(v) == false) v += 2;

    return v;
}

Hash criaHash(int tam, bool gerarPrimo, double fPreenchimento){
    if(tam < 1){
        printf("\n - criaHash() -> Tamanho passado para criacao da tabela Hash invalido.");
        return NULL;
    }

    // Aloca uma nova tabela hash.
    HashStr* tabelaHash = (HashStr*)malloc(sizeof(HashStr));
    if(checkAllocation(tabelaHash, "Tabela Hash.")) return NULL;

    // Gera o pro'ximo numero primo a partir de tam caso 'gerarPrimo' for Verdadeiro.
    tabelaHash->tam = (gerarPrimo ? proxPrimo(tam) : tam);

    // Cria uma matriz de ce'lulas.
    tabelaHash->balde = (HashCel**)malloc(tabelaHash->tam * sizeof(HashCel*));
    if(checkAllocation(tabelaHash->balde, "Matriz de ce'lulas (balde) da tabela Hash.")){
        free(tabelaHash);
        return NULL;
    }

    // Seta todos as celulas para valores nulos.
    for(int i = 0; i < tabelaHash->tam; i++){
        tabelaHash->balde[i] = NULL;
    }

    // Atribui o fator de preenchimento maximo da tabela.
    tabelaHash->fPreenchimento = fPreenchimento;
    tabelaHash->qPreenchida = 0;

    // Retorna a tabela Hash criada.
    return tabelaHash;
}

// Usando djb2
static unsigned long int hashFunction(const char* nome){
    unsigned long hash = 5381;
	int x;
	
    while((x = *nome++)) hash = ((hash << 5) + hash) + x;

	return hash;
}

static int hashIndex(const char* nome, int tam){
    // Retorna o endereco pelo calculo da funcao Hash (djb2) com o mo'dulo do tamanho da tabela.
    return hashFunction(nome) % tam;
}

static Hash checkHashLimit(Hash hash){
    if(hash == NULL){
        printf("\n - getHashValue() -> Tabela passada apresenta valor nulo. -");
        return NULL;
    }

    HashStr* tabelaHash = (HashStr*)hash;

    // Verifica se o fator de preenchimento foi alcancado.
    if(tabelaHash->qPreenchida >= (int)(tabelaHash->tam * tabelaHash->fPreenchimento)){

        Hash newHash = criaHash(tabelaHash->tam * 2, true, tabelaHash->fPreenchimento);

        for(int i = 0; i < tabelaHash->tam; i++){
            if(tabelaHash->balde[i] == NULL) continue;
            newHash = inserirHash(newHash, tabelaHash->balde[i]->chave, tabelaHash->balde[i]->valor);
        }

        destroiHash(tabelaHash, NULL, NULL);
        return newHash;
    }

    return hash;
}

Hash inserirHash(Hash hash, const char* nome, HashItem valor){
    if(hash == NULL){
        printf("\n - inserirHash() -> Tabela passada apresenta valor nulo. -");
        return NULL;
    }

    if(nome == NULL || valor == NULL){
        printf("\n - inserirHash() -> Nome ou valor apresentam valores nulos.");
        return hash;
    }

    HashStr* tabelaHash = (HashStr*)hash;

    int i = hashIndex(nome, tabelaHash->tam);
    
    HashCel* hashCel = tabelaHash->balde[i];
    
    // Checa caso a chave esteja presente nas próximas casas da hashTable.
    for(; hashCel != NULL; hashCel = hashCel->prox){
        // Atualiza o valor da chave se encontrar a chave na estrutura.
        if(strcmp(hashCel->chave, nome) == 0){
            hashCel->valor = valor;
            return hash;
        }
    }
    
    // Caso não esteja, aloca uma nova célula da tabela na posição hashIndex.
    HashCel* newHashCel = (HashCel*)malloc(sizeof(HashCel));
    if(checkAllocation(newHashCel, "Nova ce'lula da tabela Hash.")) return hash;

    // Aloca nome da nova ce'lula da tabela.
    newHashCel->chave = (char*)malloc(strlen(nome) + 1);
    if(checkAllocation(newHashCel->chave, "Nome da nova ce'lula da tabela Hash.")){
        free(newHashCel);
        return hash;
    }
    strcpy(newHashCel->chave, nome);
    
    // Associa o valor 'a ce'lula.
    newHashCel->valor = valor;

    // Configura o ponteiro para o pro'ximo indice da tabela.
    newHashCel->prox = tabelaHash->balde[i];
    tabelaHash->balde[i] = newHashCel;

    tabelaHash->qPreenchida += 1;

    return checkHashLimit(tabelaHash);
}

HashItem getHashValue(Hash hash, const char* nome){
    if(hash == NULL){
        printf("\n - getHashValue() -> Tabela passada apresenta valor nulo. -");
        return NULL;
    }

    if(nome == NULL){
        printf("\n - getHashValue() -> Nome especificado apresenta valor nulo. -");
        return NULL;
    }

    HashStr* tabelaHash = (HashStr*)hash;

    // Encontra o index associado ao nome na tabela.
    int i = hashIndex(nome, tabelaHash->tam);

    // Itera sobre as ce'lulas do array balde.
    for(HashCel* hashCel = tabelaHash->balde[i]; hashCel != NULL; hashCel = hashCel->prox){
        // Compara o nome passado com o nome associado 'as ce'lulas 'a partir do index encontrado.
        if(strcmp(hashCel->chave, nome) == 0) return hashCel->valor;
    }

    // Caso o nome nao for encontrado, retorna nulo.
    return NULL;
}

void destroiHash(Hash hash, freeFunc fFunc, void* extra){
    if(hash == NULL){
        printf("\n - destroiHash() -> Tabela hash apresenta valor nulo. -");
        return;
    }

    // if(fFunc == NULL){
    //     printf("\n - destroiHash() -> Funcao de limpeza apresenta valor nulo. -");
    //     return;
    // }

    HashStr* tabelaHash = (HashStr*)hash;

    // Itera sobre o array de ce'lulas da tabela.
    for(int i = 0; i < tabelaHash->tam; i++){
        HashCel* cel = tabelaHash->balde[i];
        
        // Enquanto ce'lula for diferente de nulo, libera todas as informacoes associadas 'a ce'lula.
        while(cel){
            HashCel* prox = cel->prox;

            free(cel->chave);

            if(fFunc != NULL) fFunc(cel->valor, extra);

            free(cel);

            cel = prox;
        }
    }

    // Libera o array de ce'lulas e a pro'pria tabela Hash.
    free(tabelaHash->balde);
    free(tabelaHash);
}

// DEBUG FUNCTIONS

void imprimirHash(Hash hash, printFunc pFunc){
    HashStr* tabelaHash = (HashStr*)hash;

    for(int i = 0; i < tabelaHash->tam; i++){
        if(tabelaHash->balde[i] == NULL) continue;

        printf("\n [%d]: %s : %s", i, tabelaHash->balde[i]->chave, pFunc(tabelaHash->balde[i]->valor, NULL));
    }
}
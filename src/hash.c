#include "hash.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct HashCel{
    char* chave;
    int valor;

    struct HashCel* prox;
} HashCel;

typedef struct HashStr{
    int tam;
    HashCel** balde;
} HashStr;

bool isPrimo(int v){
    int raizV = sqrt(v);
    for(int i = 2; i <= raizV; i++){
        if(v % i == 0) return false;
    }
    return true;
}

int proxPrimo(int v){
    v += (v % 2 == 0 ? 1 : 2);
    while(isPrimo(v) == false) v += 2;

    return v;
}

Hash criaHash(int tam, bool gerarPrimo){
    HashStr* tabelaHash = (HashStr*)malloc(sizeof(HashStr));

    tabelaHash->tam = (gerarPrimo ? proxPrimo(tam) : tam);

    tabelaHash->balde = (HashCel**)malloc(tabelaHash->tam * sizeof(HashCel*));

    for(int i = 0; i < tabelaHash->tam; i++){
        tabelaHash->balde[i] = NULL;
    }

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
    return hashFunction(nome) % tam;
}

void inserirHash(Hash hash, const char* nome, int valor){
    HashStr* tabelaHash = (HashStr*)hash;
    int i = hashIndex(nome, tabelaHash->tam);
    
    HashCel* hashCel = tabelaHash->balde[i];
    
    // Checa caso a chave esteja presente nas próximas casas da hashTable.
    for(; hashCel != NULL; hashCel = hashCel->prox){
        // Atualiza o valor da chave se encontrar a chave na estrutura.
        if(strcmp(hashCel->chave, nome) == 0){
            hashCel->valor = valor;
            return;
        }
    }
    
    // Caso não esteja, aloca uma nova célula da tabela na posição hashIndex.
    HashCel* newHashCel = (HashCel*)malloc(sizeof(HashCel));
    newHashCel->chave = (char*)malloc(strlen(nome) + 1);

    strcpy(newHashCel->chave, nome);
    newHashCel->valor = valor;

    newHashCel->prox = tabelaHash->balde[i];
    tabelaHash->balde[i] = newHashCel;
}

int getHashValue(Hash hash, const char* nome){
    HashStr* tabelaHash = (HashStr*)hash;

    int i = hashIndex(nome, tabelaHash->tam);

    HashCel* hashCel = tabelaHash->balde[i];

    for(; hashCel != NULL; hashCel = hashCel->prox){
        if(strcmp(hashCel->chave, nome) == 0) return hashCel->valor;
    }

    return -1;
}

void destroiHash(Hash hash){
    HashStr* tabelaHash = (HashStr*)hash;

    for(int i = 0; i < tabelaHash->tam; i++){
        HashCel* cel = tabelaHash->balde[i];
        
        while(cel){
            HashCel* prox = cel->prox;

            free(cel->chave);
            free(cel);

            cel = prox;
        }
    }

    free(tabelaHash->balde);
    free(tabelaHash);
}
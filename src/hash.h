#ifndef _HASH_h_
#define _HASH_h_

#include <stdbool.h>

// DESCREVER A ESTRUTURA.

typedef void* Hash;
typedef void* HashItem;

// Funcao para liberacao da memoria armazenada na tabela Hash.
typedef void (*freeFunc) (HashItem hashItem);

Hash criaHash(int tam, bool gerarPrimo);

void inserirHash(Hash hash, const char* nome, HashItem valor);

HashItem getHashValue(Hash hash, const char* nome);

void destroiHash(Hash hash, freeFunc fFunc);

#endif
#ifndef _HASH_h_
#define _HASH_h_

#include <stdbool.h>

typedef void* Hash;

Hash criaHash(int tam, bool gerarPrimo);

void inserirHash(Hash hash, const char* nome, int valor);

int getHashValue(Hash hash, const char* nome);

void destroiHash(Hash hash);

#endif
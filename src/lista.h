#ifndef _Lista_h_
#define _Lista_h_

#include <stdbool.h>
#include <stdlib.h>

typedef void* Lista;

Lista criaLista();

void inserirInicio(Lista, void*);

//void inserirMeio(Lista, void*, void*);

void inserirFim(Lista, void*);

void* removerInicio(Lista);

//void* removerMeio(Lista, void*);

void* removerFim(Lista);

void* remover(Lista, bool(*f)(const void*, const void*), const void*);

bool isListaVazia(Lista);

int listaTamanho(Lista);

void mapTo(Lista from, Lista to, void* (*mapFunction)(const void*));

void concatLista(Lista receive, Lista concatFrom, size_t itemSize);

void imprimirLista(Lista, void(*imprimir)(const void*));

//void imprimirListaRel(Lista, void*, void(*imprimir)(const void*, const void*));

//void imprimirListaRelRel(Lista, void*, void*, void(*imprimir)(const void*, const void*, const void*));

void percorrerLista(Lista, void(*f)(const void*));

void percorrerListaRel(Lista, void(*f)(const void*, const void*), void*);

void percorrerListaRelRel(Lista, void(*f)(const void*, const void*, const void*), void*, void*);

bool isInLista(Lista, bool(*f)(const void*, const void*), const void*);

void* getItemLista(Lista, int);

void* getItemListaI(Lista Lista, void* element, bool(*f)(const void*, const void*));

void limparLista(Lista, bool);

void copyLista(Lista from, Lista to, size_t itemSize);

#endif
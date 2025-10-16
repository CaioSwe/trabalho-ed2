#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct cel{
    void* item;
    struct cel* prox;
    struct cel* ant;
} Celula;

typedef struct Lista{
    Celula* inicio;
    Celula* fim;
    int tamanho;
} listaStr;

Lista criaLista(){
    listaStr* l = (listaStr*)malloc(sizeof(listaStr));
    if(l == NULL){
        printf("\n- criaLista() -> Erro na alocação de memória da lista. -");
        return NULL;
    }

    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
    return l;
}

Celula* criaCelula(){
    Celula* celula = (Celula*)malloc(sizeof(Celula));

    if(celula == NULL){
        printf("\n- criaCelula() -> Erro na alocação de memória da célula. -");
        return NULL;
    }

    return celula;
}

void inserirInicio(Lista l, void* v){
    Celula* nova = criaCelula();
    nova->item = v;

    listaStr* lista = (listaStr*)l;

    if (lista->inicio == NULL){ // lista vazia
        lista->inicio = nova;
        lista->fim = nova;
        nova->prox = NULL;
        nova->ant = NULL;
    }
    else{
        nova->prox = lista->inicio;
        nova->ant = NULL;
        lista->inicio->ant = nova;
        lista->inicio = nova;
    }

    lista->tamanho += 1;
}

void inserirFim(Lista l, void* v){
    Celula* nova = criaCelula();
    nova->item = v;

    listaStr* lista = (listaStr*)l;

    if (lista->inicio == NULL){
        lista->inicio = nova;
        lista->fim = nova;
        nova->prox = NULL;
        nova->ant = NULL;
    }
    else{
        nova->prox = NULL;
        lista->fim->prox = nova;
        nova->ant = lista->fim;
        lista->fim = nova;
    }

    lista->tamanho += 1;
}

void* removerInicio(Lista l){
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->inicio;
    void* v = cel->item;

    lista->inicio = lista->inicio->prox;

    if(lista->inicio == NULL) // lista ficará vazia
        lista->fim = NULL;
    else
        lista->inicio->ant = NULL;
    
    free(cel);

    lista->tamanho -= 1;

    return v;
}

void* removerFim(Lista l){
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->fim;
    void* v = cel->item;

    lista->fim = lista->fim->ant;

    free(cel);

    lista->tamanho -= 1;

    return v;
}

void* remover(Lista l, bool(*f)(const void*, const void*), const void* item){
    listaStr* lista = (listaStr*)l;
    
    if (!lista) return NULL;

    Celula* cel = lista->inicio;
    Celula* aux = NULL;

    if(f(cel->item, item)){
        lista->tamanho -= 1;
        lista->inicio = cel->prox;
        return cel->item;
    }

    while(cel->prox != NULL){
        if(f(cel->prox->item, item)){
            lista->tamanho -= 1;
            aux = cel->prox;
            cel->prox = cel->prox->prox;
            return aux->item;
        }
        cel = cel->prox;
    }

    return NULL;
}

bool isListaVazia(Lista l){
    listaStr* lista = (listaStr*)l;

    if(lista->inicio == NULL){
        return true;
    }
    return false;
}

int listaTamanho(Lista l){
    return ((listaStr*)l)->tamanho;
}

void percorrerLista(Lista l, void (*f)(const void*)) {
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        f(cel->item);
        cel = cel->prox;
    }
}

void percorrerListaRel(Lista l, void (*f)(const void*, const void*), void* aux) {
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        f(cel->item, aux);
        cel = cel->prox;
    }
}

bool isInLista(Lista l, bool(*f)(const void*, const void*), const void* valor) {
    listaStr* lista = (listaStr*)l;

    if (!lista) return false;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        if(f(cel->item, valor)){
            return true;
        }
        cel = cel->prox;
    }
    return false;
}

void* getItemLista(Lista l, int pos){
    listaStr* lista = (listaStr*)l;

    if (!lista || lista->tamanho < pos) return NULL;

    Celula* cel = lista->inicio;

    for(int i = 0; (i < pos) && (cel != NULL); i++){
        cel = cel->prox;
    }

    return cel->item;
}

void* getItemListaI(Lista l, void* element, bool(*f)(const void*, const void*)){
    listaStr* lista = (listaStr*)l;

    if (!lista) return false;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        if(f(cel->item, element)){
            return cel->item;
        }
        cel = cel->prox;
    }
    return NULL;
}

void imprimirLista(Lista l, void(*imprimir)(const void*)){
    listaStr* lista = (listaStr*)l;

    if (!lista) return;

    Celula* cel = lista->inicio;

    while(cel != NULL){
        imprimir(cel->item);
        cel = cel->prox;
    }
}

void limparLista(Lista l, bool freeItems) {
    listaStr* lista = (listaStr*)l;

    if (!lista) return;

    Celula* atual = lista->inicio;
    while (atual != NULL) {
        Celula* temp = atual;
        atual = atual->prox;
        if(freeItems) free(temp->item);
        free(temp);
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

void copyLista(Lista f, Lista t){
    listaStr* from = (listaStr*)f;
    listaStr* to = (listaStr*)t;

    limparLista(to, false);

    to->inicio = from->inicio;
    to->fim = from->fim;
    to->tamanho = from->tamanho;
}
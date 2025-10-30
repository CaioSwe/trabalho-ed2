#include "lista.h"
#include <stdio.h>
#include <string.h>

// CONSERTAR ORDEM DOS PARAMETROS DAS FUNÇÕES DE CALLBACK (inconsistente)

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

static Celula* criaCelula(){
    Celula* celula = (Celula*)malloc(sizeof(Celula));

    if(celula == NULL){
        printf("\n- criaCelula() -> Erro na alocação de memória da célula. -");
        return NULL;
    }

    return celula;
}

void inserirInicio(Lista l, Item v){
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

void inserirFim(Lista l, Item v){
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

Item removerInicio(Lista l){
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->inicio;
    void* v = cel->item;

    lista->inicio = lista->inicio->prox;

    if(lista->inicio == NULL)
        lista->fim = NULL;
    else
        lista->inicio->ant = NULL;
    
    free(cel);

    lista->tamanho -= 1;

    return v;
}

Item removerFim(Lista l){
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->fim;
    void* v = cel->item;

    lista->fim = lista->fim->ant;

    if (lista->fim) lista->fim->prox = NULL;
    else lista->inicio = NULL;
    
    free(cel);

    lista->tamanho -= 1;

    return v;
}

Item remover(Lista l, compararItens compFunc, Item item){
    listaStr* lista = (listaStr*)l;
    
    if (!lista) return NULL;

    Celula* cel = lista->inicio;

    if(compFunc(cel->item, item)){
        lista->inicio = cel->prox;

        if (lista->inicio) lista->inicio->ant = NULL;
        else lista->fim = NULL;
        
        void* itemV = cel->item;
        
        free(cel);
        lista->tamanho -= 1;
        
        return itemV;
    }

    while(cel->prox != NULL){
        if(compFunc(cel->prox->item, item)){
            Celula* toRemove = cel->prox;
            cel->prox = toRemove->prox;

            if(toRemove->prox) toRemove->prox->ant = cel;
            else lista->fim = cel;

            void* ret = toRemove->item;
            
            free(toRemove);
            
            lista->tamanho--;
            
            return ret;
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

void mapTo(Lista from, Lista to, mapFunction mapFunc){
    listaStr* listaFrom = (listaStr*)from;
    listaStr* listaTo = (listaStr*)to;

    if (!listaFrom || !listaTo) return;

    limparLista(listaTo, false);

    Celula* cel = listaFrom->inicio;
    
    while(cel != NULL){
        inserirFim(listaTo, mapFunc(cel->item));
        cel = cel->prox;
    }
}

void concatLista(Lista receive, Lista concatFrom, size_t itemSize){
    listaStr* listaR = (listaStr*)receive;
    listaStr* listaCF = (listaStr*)concatFrom;

    if(!listaR || !listaCF) return;

    if(listaR->inicio == NULL) copyLista(concatFrom, receive, itemSize);

    Celula* atual = listaCF->inicio;

    while (atual != NULL) {
        void* newItem = malloc(itemSize);
        memcpy(newItem, atual->item, itemSize);
        inserirFim(listaR, newItem);

        atual = atual->prox;
    }
}

void percorrerLista(Lista l, runThroughItems runFunc, void* extra) {
    listaStr* lista = (listaStr*)l;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        runFunc(cel->item, extra);
        cel = cel->prox;
    }
}

bool isInLista(Lista l, compararItens compFunc, Item item) {
    listaStr* lista = (listaStr*)l;

    if (!lista) return false;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        if(compFunc(cel->item, item)){
            return true;
        }
        cel = cel->prox;
    }
    return false;
}

Item getItemLista(Lista l, int index){
    listaStr* lista = (listaStr*)l;

    if (!lista || index < 0 || index >= lista->tamanho) return NULL;

    Celula* cel = lista->inicio;

    for(int i = 0; (i < index) && (cel != NULL); i++){
        cel = cel->prox;
    }

    return cel->item;
}

void* getItemListaI(Lista l, compararItens compFunc, Item item){
    listaStr* lista = (listaStr*)l;

    if (!lista) return NULL;

    Celula* cel = lista->inicio;
    
    while(cel != NULL){
        if(compFunc(cel->item, item)){
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

void limparLista(Lista l, bool limparItens) {
    listaStr* lista = (listaStr*)l;

    if (!lista) return;

    Celula* atual = lista->inicio;
    while (atual != NULL) {
        Celula* temp = atual;
        atual = atual->prox;
        if(limparItens) free(temp->item);
        free(temp);
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

void copyLista(Lista f, Lista t, size_t itemSize){
    listaStr* from = (listaStr*)f;
    listaStr* to = (listaStr*)t;

    limparLista(to, false);

    Celula* atual = from->inicio;

    while (atual != NULL) {
        void* newItem = malloc(itemSize);
        memcpy(newItem, atual->item, itemSize);
        inserirFim(to, newItem);

        atual = atual->prox;
    }
}
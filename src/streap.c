#include "streap.h"

#include <stdlib.h>
#include <stdio.h>

// #include <graphviz/gvc.h>
// #include <graphviz/cgraph.h>

/*
    TERMINAR
*/

typedef struct BoundingBox{
    double x1, y1;
    double x2, y2;
} BoundingBox;

typedef struct NodeStr{
    double x, y;        // Chave de busca
    int prioridade;
    Info info;
    
    BoundingBox* box;

    struct NodeStr* pai;
    struct NodeStr* esq;
    struct NodeStr* dir;
} NodeStr;

typedef struct STreapStr{
    NodeStr* raiz;
    int altura;
    int nRoot;

    double epsilon;
}STreapStr;

STreap createSTrp(double epsilon){
    STreapStr* st = (STreapStr*)malloc(sizeof(STreapStr));
    if(!st) return NULL;

    st->raiz = NULL;
    st->altura = 0;
    st->nRoot = 0;
    st->epsilon = epsilon;

    return (STreap)st;
}

Node insertSTrp(STreap t, double x, double y, Info info){
    STreapStr* st = (STreapStr*)t;

    NodeStr* newNode = (NodeStr*)malloc(sizeof(NodeStr));

    newNode->dir = NULL;
    newNode->esq = NULL;
    newNode->info = info;

    newNode->x = x;
    newNode->y = y;

    newNode->box = (BoundingBox*)malloc(sizeof(BoundingBox));

    if(st->raiz == NULL){
        st->raiz = newNode;
        return st->raiz;
    }

    //////////////////////////// ????????????

    return NULL;
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado);

Info getInfoSTrp(STreap t, Node n){
    return ((NodeStr*)n)->info;
}

Node getNodeSTrp(STreap t, double xa, double ya);

void updateInfoSTrp(STreap t, Node n, Info i){
    ((NodeStr*)n)->info = i;
}

Info deleteNodeSTrp(STreap t, Node n){
    NodeStr* root = ((NodeStr*)n);

    Info i = root->info;

    // tirar N da streap

    free(root);
    return i;
}

Info removeSTrp(STreap t, double xa, double ya);

void printSTrp(STreap t, char *nomeArq);

void percursoLargura(STreap t, FvisitaNo fVisita, void *aux);

void percursoSimetrico(STreap t, FvisitaNo fVisita, void *aux);

void percursoProfundidade(STreap t, FvisitaNo fVisita, void *aux);

void killSTrp(STreap t);
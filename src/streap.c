#include "streap.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// #include <graphviz/gvc.h>
// #include <graphviz/cgraph.h>

/*
    TERMINAR
*/

// Verificar condicao de insercao 'a direita.
// Implementar sorteio da pioridade do Node.
// Implementar BoundingBox

// Funcoes para terminar:
// removeSTrpRec
// prioridade
// insertNodeByKeySTrp (eu acho)
// insertSTrp
// getNodeRegiaoSTrp
// deleteNodeSTrp
// etc

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

static NodeStr* createNodeSTrp(double x, double y, Info info){
    NodeStr* newNode = (NodeStr*)malloc(sizeof(NodeStr));

    newNode->box = (BoundingBox*)malloc(sizeof(BoundingBox));

    newNode->pai = NULL;
    newNode->dir = NULL;
    newNode->esq = NULL;

    newNode->info = info;

    newNode->x = x;
    newNode->y = y;

    return newNode;
}

static void rotateRightSTrp(Node node){
    NodeStr* p = (NodeStr*)node;
    NodeStr* q = p->esq;
    NodeStr* aux = q->dir;

    q->dir = p;
    p->esq = aux;

    q->pai = p->pai;
    p->pai = q;
    aux->pai = p;
}

static void rotateLeftSTrp(Node node){
    NodeStr* p = (NodeStr*)node;
    NodeStr* q = p->dir;
    NodeStr* aux = q->esq;

    q->esq = p;
    p->dir = aux;

    q->pai = p->pai;
    p->pai = q;
    aux->pai = p;
}

static Node insertNodeByKeySTrp(STreap t, double x, double y, Info info){
    STreapStr* st = (STreapStr*)t;

    NodeStr* newNode = createNodeSTrp(x, y, info);

    if(st->raiz == NULL){
        st->raiz = newNode;
        return newNode;
    }

    NodeStr* aux = st->raiz;

    // ch1 < ch2, se:
    // (ch1.x < ch2.x)    
    //  -- OU --
    // (ch1.x == ch2.x AND ch1.y < ch2.y)
    
    // Verificacao de igualdade: |v - w| <= epsilon.
    while(aux != NULL){
        if((newNode->x < aux->x) || (fabs(newNode->x - aux->x) <= st->epsilon && newNode->y < aux->y)){
            if(aux->esq == NULL){
                aux->esq = newNode;
                newNode->pai = aux;
            }
            else aux = aux->esq;
        }
        else if(!(fabs(newNode->x - aux->x) <= st->epsilon && fabs(newNode->y - aux->y) <= st->epsilon)){
            if(aux->dir == NULL){
                aux->dir = newNode;
                newNode->pai = aux;
            }
            else aux = aux->dir;
        }
        else{
            printf("\n -- insertNodeByKeySTrp() -> No' ja' existe na arvore. -- ");
            return NULL;
        }
    }
}

static void rebalancearSTrp(NodeStr* node){
    NodeStr* pai = node->pai;

    if(pai == NULL) return;

    if(pai->prioridade < node->prioridade){
        if(pai->esq == node) rotateRightSTrp(pai);
        else rotateLeftSTrp(pai);

        rebalancearSTrp(node);
    }
}

Node insertSTrp(STreap t, double x, double y, Info info){
    NodeStr* newNode = insertNodeByKeySTrp(t, x, y, info);

    // Sortear prioridade de newNode.

    rebalancearSTrp(newNode);

    // Calcular novo BoundingBox

    return newNode;
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado);

Info getInfoSTrp(STreap t, Node n){
    return ((NodeStr*)n)->info;
}

static Node getNodeSTrpRec(NodeStr* node, double x, double y, double epsilon){
    if(node == NULL) return NULL;
    
    if((node->x < x) || (fabs(node->x - x) <= epsilon && node->y < y)){
        return getNodeSTrpRec(node->esq, x, y, epsilon);
    }
    else{
        return node;
    }
}

Node getNodeSTrp(STreap t, double xa, double ya){
    STreapStr* tr = (STreapStr*)t;
    
    if(tr == NULL) return NULL;

    return getNodeSTrpRec(tr->raiz, xa, ya, tr->epsilon);
}

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

static NodeStr* removeSTrpRec(NodeStr* node){
    if(!(node->esq && node->dir)) return node;

    if(node->esq->prioridade > node->dir->prioridade){

    }
}

Info removeSTrp(STreap t, double xa, double ya){
    NodeStr* node = getNodeSTrp(t, xa, ya);

    NodeStr* aux = removeSTrpRec(node);
    Info i = aux->info;

    if(aux->dir){
        if(aux->pai->dir == aux) aux->pai->dir = aux->dir;
        else aux->pai->esq = aux->dir;
        
        aux->dir->pai = aux->pai;
    }
    else if(aux->esq){
        if(aux->pai->dir == aux) aux->pai->dir = aux->esq;
        else aux->pai->esq = aux->esq;
        
        aux->esq->pai = aux->pai;
    }
    else{
        free(aux->box);
        free(aux);
    }

    return i;
}

void printSTrp(STreap t, char *nomeArq);

void percursoLargura(STreap t, FvisitaNo fVisita, void *aux);

void percursoSimetrico(STreap t, FvisitaNo fVisita, void *aux);

void percursoProfundidade(STreap t, FvisitaNo fVisita, void *aux);

void killSTrp(STreap t);
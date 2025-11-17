#include "streap.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <time.h>

// #include <graphviz/gvc.h>
// #include <graphviz/cgraph.h>

/*
    TERMINAR
*/

// Verificar condicao de insercao 'a direita.

typedef struct BoundingBox{
    double x1, y1;
    double x2, y2;
} BoundingBox;

typedef struct NodeStr{
    double x, y;        // Chave de busca
    Info info;
    BoundingBox* box;

    int prioridade;

    struct NodeStr* pai;
    struct NodeStr* esq;
    struct NodeStr* dir;
} NodeStr;

typedef struct STreapStr{
    NodeStr* raiz;
    int altura;
    int nRoot;

    double epsilon;

    freeFunc fFunc;
}STreapStr;

// Declaracao de escopo de funcoes.
static void freeNodeSTrp(NodeStr* node, freeFunc fFunc);

STreap createSTrp(double epsilon){
    STreapStr* st = (STreapStr*)malloc(sizeof(STreapStr));
    if(checkAllocation(st, "STreap.")) return NULL;

    st->raiz = NULL;
    st->altura = 0;
    st->nRoot = 0;
    st->epsilon = epsilon;

    st->fFunc = NULL;

    // Geracao de prioridade aleatoria.
    srand((unsigned)time(NULL));

    return (STreap)st;
}

static NodeStr* createNodeSTrp(double x, double y, Info info){
    NodeStr* newNode = (NodeStr*)malloc(sizeof(NodeStr));
    if(checkAllocation(newNode, "Novo node STreap.")) return NULL;

    newNode->box = (BoundingBox*)malloc(sizeof(BoundingBox));
    if(checkAllocation(newNode->box, "Bounding box do novo node da STreap.")){
        free(newNode);
        return NULL;
    }

    newNode->box->x1 = x;
    newNode->box->x2 = x;
    newNode->box->y1 = y;
    newNode->box->y2 = y;

    newNode->pai = NULL;
    newNode->dir = NULL;
    newNode->esq = NULL;

    newNode->info = info;

    newNode->x = x;
    newNode->y = y;

    newNode->prioridade = rand();

    return newNode;
}

static void updateBoundingBox(NodeStr* node){
    if(node == NULL) return;

    node->box->x1 = node->x;
    node->box->x2 = node->x;
    node->box->y1 = node->y;
    node->box->y2 = node->y;

    if(node->esq != NULL){
        node->box->x1 = fmin(node->box->x1, node->esq->box->x1);
        node->box->x2 = fmax(node->box->x2, node->esq->box->x2);
        node->box->y1 = fmin(node->box->y1, node->esq->box->y1);
        node->box->y2 = fmax(node->box->y2, node->esq->box->y2);
    }
    if(node->dir != NULL){
        node->box->x1 = fmin(node->box->x1, node->dir->box->x1);
        node->box->x2 = fmax(node->box->x2, node->dir->box->x2);
        node->box->y1 = fmin(node->box->y1, node->dir->box->y1);
        node->box->y2 = fmax(node->box->y2, node->dir->box->y2);
    }
}

static void updateBoundingBoxUp(NodeStr* node){
    while(node != NULL){
        updateBoundingBox(node);
        node = node->pai;
    }
}

static void rotateRightSTrp(STreap t, NodeST node){
    if(node == NULL){
        printf("\n - rotateRightSTrp() -> NodeST passado e' nulo.");
        return;
    }

    NodeStr* p = (NodeStr*)node;
    NodeStr* q = p->esq;

    if(q == NULL) return;

    NodeStr* aux = q->dir;

    q->dir = p;
    p->esq = aux;

    q->pai = p->pai;
    p->pai = q;
    if(aux != NULL) aux->pai = p;

    if(q->pai){
        if(q->pai->esq == p) q->pai->esq = q;
        else if(q->pai->dir == p) q->pai->dir = q;
    }
    else{
        STreapStr* st = (STreapStr*)t;
        st->raiz = q;
    }

    updateBoundingBox(p);
    updateBoundingBoxUp(q);
}

static void rotateLeftSTrp(STreap t, NodeST node){
    if(node == NULL){
        printf("\n - rotateLeftSTrp() -> NodeST passado e' nulo.");
        return;
    }

    NodeStr* p = (NodeStr*)node;
    NodeStr* q = p->dir;

    if(q == NULL) return;

    NodeStr* aux = q->esq;

    q->esq = p;
    p->dir = aux;

    q->pai = p->pai;
    p->pai = q;
    if(aux != NULL) aux->pai = p;

    if(q->pai){
        if(q->pai->esq == p) q->pai->esq = q;
        else if(q->pai->dir == p) q->pai->dir = q;
    }
    else{
        STreapStr* st = (STreapStr*)t;
        st->raiz = q;
    }

    updateBoundingBox(p);
    updateBoundingBoxUp(q);
}

static NodeST insertNodeByKeySTrp(STreap t, double x, double y, Info info){
    if(t == NULL){
        printf("\n - insertNodeByKeySTrp() -> Treap passada e' nula. -");
        return NULL;
    }
    
    STreapStr* st = (STreapStr*)t;

    NodeStr* newNode = createNodeSTrp(x, y, info);
    if(newNode == NULL) return NULL;

    updateBoundingBoxUp(newNode);

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
        if((x < aux->x) || ((fabs(x - aux->x) <= st->epsilon) && (y < aux->y))){
            if(aux->esq == NULL){
                aux->esq = newNode;
                newNode->pai = aux;
                break;
            }
            else aux = aux->esq;
        }
        else if(!((fabs(x - aux->x) <= st->epsilon) && (fabs(y - aux->y) <= st->epsilon))){
            if(aux->dir == NULL){
                aux->dir = newNode;
                newNode->pai = aux;
                break;
            }
            else aux = aux->dir;
        }
        else{
            freeNodeSTrp(newNode, st->fFunc);
            printf("\n -- insertNodeByKeySTrp() -> No' ja' existe na arvore. -- ");
            return NULL;
        }
    }

    return newNode;
}

static void rebalancearSTrp(STreap t, NodeStr* node){
    if(node == NULL){
        printf("\n - rebalancearSTrp() -> NodeST passado e' nulo. -");
        return;
    }

    NodeStr* pai = node->pai;

    if(pai == NULL) return;

    if(pai->prioridade < node->prioridade){
        if(pai->esq == node) rotateRightSTrp(t, pai);
        else rotateLeftSTrp(t, pai);

        rebalancearSTrp(t, node);
    }
}

NodeST insertSTrp(STreap t, double x, double y, Info info){
    if(t == NULL){
        printf("\n - insertSTrp() -> Treap passada e' nula. - ");
        return NULL;
    }

    NodeStr* newNode = insertNodeByKeySTrp(t, x, y, info);
    if(newNode == NULL) return NULL;

    rebalancearSTrp(t, newNode);

    updateBoundingBoxUp(newNode);

    return newNode;
}

// Checa se todos os pontos de b2 estao completamente dentro de b1.
static bool checkInsideBoxes(BoundingBox b1, BoundingBox b2){
    return (b2.x1 >= b1.x1 && b2.x2 <= b1.x2 && b2.y1 >= b1.y1 && b2.y2 <= b1.y2);
}

// Checa se os pontos de b2 estao completamente fora de b1.
static bool checkOutsideBoxes(BoundingBox b1, BoundingBox b2){
    return (b2.x2 < b1.x1 || b2.x1 > b1.x2 || b2.y2 < b1.y1 || b2.y1 > b1.y2);
}

static void getNodeRegiaoSTrpRec(STreapStr* tr, NodeStr* node, BoundingBox rec, Lista resultado){
    if(node == NULL) return;

    // insere o node atual na lista se estiver dentro da regiao.
    if(checkInsideBoxes(rec, *node->box)) inserirInicio(resultado, node);

    // Checa se o no' esquerdo ou direito estao completamente/parcialmente dentro da regiao.
    // Se estiverem, percorre os no's dentro das suba'rvores.
    if(node->esq != NULL && !checkOutsideBoxes(rec, *node->esq->box)){
        getNodeRegiaoSTrpRec(tr, node->esq, rec, resultado);
    }
    if(node->dir != NULL && !checkOutsideBoxes(rec, *node->dir->box)){
        getNodeRegiaoSTrpRec(tr, node->dir, rec, resultado);
    }
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado){
    if(t == NULL){
        printf("\n - getNodeRegiaoSTrp() -> Treap passada e' nula. -");
        return;
    }

    STreapStr* tr = (STreapStr*)t;

    if(tr->raiz == NULL){
        printf("\n - getNodeRegiaoSTrp() -> Treap vazia. -");
        return;
    }

    // Se a regiao estiver completamente fora do boundingbox da raiz, retorna a lista sem nenhuma busca/insercao.
    if(checkOutsideBoxes(*tr->raiz->box, (BoundingBox){x, y, x+w, y+h})) return;

    getNodeRegiaoSTrpRec(tr, tr->raiz, (BoundingBox){x, y, x+w, y+h}, resultado);
}

Info getInfoSTrp(STreap t, NodeST n, double *x, double *y, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2){
    NodeStr* ntr = (NodeStr*)n;
    
    if(x != NULL) *x = ntr->x;
    if(y != NULL) *y = ntr->y;
    if(mbbX1 != NULL) *mbbX1 = ntr->box->x1;
    if(mbbX2 != NULL) *mbbX2 = ntr->box->x2;
    if(mbbY1 != NULL) *mbbY1 = ntr->box->y1;
    if(mbbY2 != NULL) *mbbY2 = ntr->box->y2;

    return ntr->info;
}

static NodeST getNodeSTrpRec(NodeStr* node, double x, double y, double epsilon){
    if(node == NULL) return NULL;
    
    if((x < node->x) || ((fabs(x - node->x) <= epsilon) && (y < node->y))){
        return getNodeSTrpRec(node->esq, x, y, epsilon);
    }
    else if((node->x < x) || ((fabs(x - node->x) <= epsilon) && (node->y < y))){
        return getNodeSTrpRec(node->dir, x, y, epsilon);
    }
    else{
        return node;
    }
}

NodeST getNodeSTrp(STreap t, double xa, double ya){
    STreapStr* tr = (STreapStr*)t;
    
    if(tr == NULL) return NULL;

    return getNodeSTrpRec(tr->raiz, xa, ya, tr->epsilon);
}

void updateInfoSTrp(STreap t, NodeST n, Info i){
    ((NodeStr*)n)->info = i;
}

static void rebalancearDeleteNodeSTrp(STreapStr* tr, NodeStr* node){
    if(node == NULL || tr == NULL){
        printf("\n - rebalancearDeleteNodeSTrp() -> Uma ou mais informacoes passadas sao nulas. - ");
        return;
    }

    while(node->esq != NULL && node->dir != NULL){
        if(node->esq->prioridade > node->dir->prioridade) rotateRightSTrp(tr, node);
        else rotateLeftSTrp(tr, node);
    }

    NodeStr* filho = (node->esq) ? node->esq : node->dir;

    if(node->pai != NULL){
        if(node->pai->esq == node) node->pai->esq = filho;
        else node->pai->dir = filho;
    }
    else tr->raiz = filho;

    if(filho != NULL) filho->pai = node->pai;
}

Info deleteNodeSTrp(STreap t, NodeST n){
    if(t == NULL || n == NULL){
        printf("\n - deleteNodeSTrp() -> Uma ou mais informacoes passadas sao nulas. -");
        return NULL;
    }

    STreapStr* tr = (STreapStr*)t;
    NodeStr* root = ((NodeStr*)n);

    Info i = root->info;

    rebalancearDeleteNodeSTrp(tr, root);

    freeNodeSTrp(root, tr->fFunc);
    return i;
}

Info removeSTrp(STreap t, double xa, double ya){
    if(t == NULL){
        printf("\n - removeSTrp() -> Treap passada e' nula. -");
        return NULL;
    }

    NodeStr* node = getNodeSTrp(t, xa, ya);

    return deleteNodeSTrp(t, node);
}

void printSTrp(STreap t, char *nomeArq);

void percursoLargura(STreap t, FvisitaNo fVisita, void *aux){
    if(t == NULL){
        printf("\n - percursoLargura() -> Treap passada e' nula. -");
        return;
    }

    if(fVisita == NULL){
        printf("\n - percursoLargura() -> Funcao de visita passada e' nula. -");
        return;
    }

    STreapStr* tr = (STreapStr*)t;

    if(tr->raiz == NULL){
        printf("\n - percursoLargura() -> Treap vazia. -");
        return;
    }

    // Lista usada como uma fila.
    // (Insercao no comeco e remocao no fim)
    Lista l = criaLista();

    inserirInicio(l, tr->raiz);

    while(!isListaVazia(l)){
        NodeStr* node = removerFim(l);

        fVisita(node->info, node->x, node->y, node->box->x1, node->box->y1, node->box->x2, node->box->y2, aux);

        if(node->esq != NULL){
            inserirInicio(l, node->esq);
        }
        if(node->dir != NULL){
            inserirInicio(l, node->dir);
        }
    }
}

static void percursoSimetricoRec(NodeStr* node, FvisitaNo fVisita, void *aux){
    if(node == NULL) return;

    if(node->esq != NULL) percursoSimetricoRec(node->esq, fVisita, aux);
    if(node != NULL) fVisita(node->info, node->x, node->y, node->box->x1, node->box->y1, node->box->x2, node->box->y2, aux);
    if(node->dir != NULL) percursoSimetricoRec(node->dir, fVisita, aux);
}

void percursoSimetrico(STreap t, FvisitaNo fVisita, void *aux){
    if(t == NULL){
        printf("\n - percursoSimetrico() -> Treap passada e' nula. -");
        return;
    }

    if(fVisita == NULL){
        printf("\n - percursoSimetrico() -> Funcao de visita passada e' nula. -");
        return;
    }

    STreapStr* tr = (STreapStr*)t;

    percursoSimetricoRec(tr->raiz, fVisita, aux);
}

static void percursoProfundidadePreRec(NodeStr* node, FvisitaNo fVisita, void *aux){
    if(node == NULL) return;

    if(node != NULL) fVisita(node->info, node->x, node->y, node->box->x1, node->box->y1, node->box->x2, node->box->y2, aux);
    if(node->esq != NULL) percursoProfundidadePreRec(node->esq, fVisita, aux);
    if(node->dir != NULL) percursoProfundidadePreRec(node->dir, fVisita, aux);
}

void percursoProfundidade(STreap t, FvisitaNo fVisita, void *aux){
    if(t == NULL){
        printf("\n - percursoProfundidade() -> Treap passada e' nula. -");
        return;
    }

    if(fVisita == NULL){
        printf("\n - percursoProfundidade() -> Funcao de visita passada e' nula. -");
        return;
    }

    STreapStr* tr = (STreapStr*)t;

    percursoProfundidadePreRec(tr->raiz, fVisita, aux);
}

static void freeNodeSTrp(NodeStr* node, freeFunc fFunc){
    if(node == NULL){
        printf("\n - freeNodeSTrp() -> NodeST passado e' nulo. -");
        return;
    }

    if(fFunc != NULL) fFunc(node->info, NULL);
    free(node->box);
    free(node);
}

static void killSTrpRec(NodeStr* node, freeFunc fFunc, void* extra){
    if(node == NULL) return;

    if(node->esq != NULL) killSTrpRec(node->esq, fFunc, extra);
    if(node->dir != NULL) killSTrpRec(node->dir, fFunc, extra);
    if(node != NULL) freeNodeSTrp(node, fFunc);
}

void killSTrp(STreap t, freeFunc fFunc, void* extra){
    if(t == NULL){
        printf("\n - killSTrp() -> Treap passada e' nula. -");
        return;
    }

    STreapStr* tr = (STreapStr*)t;

    killSTrpRec(tr->raiz, fFunc, extra);
    free(tr);
}
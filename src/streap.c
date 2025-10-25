#include "streap.h"

/*
    TERMINAR
*/

typedef struct BoundingBox{
    double x;
    double y;
    double width;
    double height;
} BoundingBox;

typedef struct RootStr{
    double x, y;        // Chave
    int prioridade;
    Info info;
    
    struct RootStr* pai;
    struct RootStr* esq;
    struct RootStr* dir;
} RootStr;

typedef struct STreapStr{
    RootStr* raiz;
    int altura;
    int nRoot;
}STreapStr;

STreap createSTrp(double epsilon);

Root insertSTrp(STreap t, double x, double y,  Info info){
    STreapStr* st = (STreapStr*)t;

    return NULL;
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado);

Info getInfoSTrp(STreap t, Root n){
    return ((RootStr*)n)->info;
}

Root getNodeSTrp(STreap t, double xa, double ya);

void updateInfoSTrp(STreap t, Root n, Info i){
    ((RootStr*)n)->info = i;
}

Info deleteNodeSTrp(STreap t, Root n){
    RootStr* root = ((RootStr*)n);

    Info i = root->info;

    // tirar N da streap

    free(root);
    return i;
}

Info removeSTrp(STreap t,double xa, double ya);

void printSTrp(STreap t, char *nomeArq);

void percursoLargura(STreap t, FvisitaNo fVisita, void *aux);

void percursoSimetrico(STreap t, FvisitaNo fVisita, void *aux);

void percursoProfundidade(STreap t, FvisitaNo fVisita, void *aux);

void killSTrp(STreap t);
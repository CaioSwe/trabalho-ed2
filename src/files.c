#include <stdio.h>

#include "geo.h"
#include "files.h"
#include "digraph.h"
#include "via.h"

void printQuadrasToSVG(Quadra quadra, void* file){
    FILE* arquivo = (FILE*)file;

    fprintf(arquivo, "<rect width=\"%.1lf\" height=\"%.1lf\" x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" />\n", getQuadraWidth(quadra), getQuadraHeight(quadra), getQuadraX(quadra), getQuadraY(quadra), getQuadraCFill(quadra), getQuadraCStrk(quadra), getQuadraSW(quadra));
    fprintf(arquivo, "<text x=\"%.1lf\" y=\"%.1lf\" dominant-baseline=\"hanging\">%s</text>", getQuadraX(quadra), getQuadraY(quadra), getQuadraID(quadra));
}

void printVerticesToSVG(Item item, void* file){
    FILE* arquivo = (FILE*)file;

    double x = getVerticeViaX(item);
    double y = getVerticeViaY(item);

    fprintf(arquivo, " <circle cx=\"%.1lf\" cy=\"%.1lf\" r=\"5\" fill=\"blue\" />\n", x, y);
}

void printEdgesToSVG(Item item, void* file){
    FILE* arquivo = (FILE*)file;

    Node f = getFromNode(NULL, item);
    Node t = getToNode(NULL, item);

    Info infoF = getNodeInfo(NULL, f);
    Info infoT = getNodeInfo(NULL, t);

    double x1 = getVerticeViaX(infoF);
    double y1 = getVerticeViaY(infoF);

    double x2 = getVerticeViaX(infoT);
    double y2 = getVerticeViaY(infoT);

    fprintf(arquivo, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" style=\"stroke:red;stroke-width:2\"/>\n", x1, y1, x2, y2);
}

// typedef struct teste{
//     Graph g;
//     FILE* file;
// }teste;

// void printToTXT(Item item, void* extra){
//     teste* t = (teste*)extra;
    
//     FILE* arquivo = t->file;
//     Graph g = t->g;

//     Edge e = (Edge)item;
//     Node node = getToNode(NULL, e);

//     fprintf(arquivo, "%-20s, ", getNodeName(g, node));
// }
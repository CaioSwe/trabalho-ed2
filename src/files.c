#include <stdio.h>

#include "geo.h"
#include "files.h"
#include "digraph.h"
#include "via.h"

void printQuadraToSVG(Quadra quadra, FILE* file){
    fprintf(file, "<rect width=\"%.1lf\" height=\"%.1lf\" x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" />\n", getQuadraWidth(quadra), getQuadraHeight(quadra), getQuadraX(quadra), getQuadraY(quadra), getQuadraCFill(quadra), getQuadraCStrk(quadra), getQuadraSW(quadra));
    fprintf(file, "<text x=\"%.1lf\" y=\"%.1lf\" dominant-baseline=\"hanging\">%s</text>", getQuadraX(quadra), getQuadraY(quadra), getQuadraID(quadra));
}

void printVerticeToSVG(Graph g, Node node, FILE* file, const char* color){
    FILE* arquivo = (FILE*)file;

    Info nodeInfo = getNodeInfo(g, node);

    double x = getVerticeViaX(nodeInfo);
    double y = getVerticeViaY(nodeInfo);

    fprintf(arquivo, " <circle cx=\"%.1lf\" cy=\"%.1lf\" r=\"5\" fill=\"%s\" />\n", x, y, color);
}

void printEdgeToSVG(Graph g, Edge e, FILE* file, const char* color){
    Node f = getFromNode(g, e);
    Node t = getToNode(g, e);

    Info infoF = getNodeInfo(g, f);
    Info infoT = getNodeInfo(g, t);

    double x1 = getVerticeViaX(infoF);
    double y1 = getVerticeViaY(infoF);

    double x2 = getVerticeViaX(infoT);
    double y2 = getVerticeViaY(infoT);

    fprintf(file, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" style=\"stroke:%s;stroke-width:4\"/>\n", x1, y1, x2, y2, color);
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
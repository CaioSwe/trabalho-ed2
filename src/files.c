#include <stdio.h>

#include "geo.h"
#include "files.h"
#include "lista.h"
#include "digraph.h"
#include "via.h"

void printQuadrasToSVG(Quadra quadra, const void* file){
    FILE* arquivo = (FILE*)file;

    fprintf(arquivo, "<rect width=\"%.1lf\" height=\"%.1lf\" x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" />\n", getQuadraWidth(quadra), getQuadraHeight(quadra), getQuadraX(quadra), getQuadraY(quadra), getQuadraCFill(quadra), getQuadraCStrk(quadra), getQuadraSW(quadra));
    fprintf(arquivo, "<text x=\"%.1lf\" y=\"%.1lf\" dominant-baseline=\"hanging\">%s</text>", getQuadraX(quadra), getQuadraY(quadra), getQuadraID(quadra));
}

void printVerticesToSVG(const void* item, const void* file){
    FILE* arquivo = (FILE*)file;

    double x = getVerticeViaX(item);
    double y = getVerticeViaY(item);

    fprintf(arquivo, " <circle cx=\"%.1lf\" cy=\"%.1lf\" r=\"5\" fill=\"blue\" />\n", x, y);
}

void printToTXT(const void* item, const void* file, const void* aux){
    FILE* arquivo = (FILE*)file;
    Graph g = (Graph)aux;

    Edge e = (Edge)item;
    Node node = getToNode(NULL, e);

    fprintf(arquivo, "%-20s, ", getNodeName(g, node));
}
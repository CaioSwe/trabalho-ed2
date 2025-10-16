#include <stdio.h>

#include "geo.h"
#include "svg.h"

void printToSVG(const void* item, const void* file){
    FILE* arquivo = (FILE*)file;

    fprintf(arquivo, "<rect width=\"%.1lf\" height=\"%.1lf\" x=\"%.1lf\" y=\"%.1lf\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" />\n", getQuadraWidth(item), getQuadraHeight(item), getQuadraX(item), getQuadraY(item), getQuadraCFill(item), getQuadraCStrk(item), getQuadraSW(item));
}
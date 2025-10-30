#ifndef _FILES_h_
#define _FILES_h_

#include "geo.h"

void printQuadrasToSVG(Quadra quadra, const void* file);

void printVerticesToSVG(const void* item, const void* file);

void printToTXT(const void* item, const void* file, const void* aux);

#endif
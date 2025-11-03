#ifndef _VIA_h_
#define _VIA_h_

#include "digraph.h"

typedef void* VerticeVia;
typedef void* ArestaVia;

Graph processViaFile(const char* path);

double getVerticeViaX(VerticeVia vv);

double getVerticeViaY(VerticeVia vv);

bool isArestaEnabled(ArestaVia av);

void blockAresta(ArestaVia av);

void unblockAresta(ArestaVia av);

#endif
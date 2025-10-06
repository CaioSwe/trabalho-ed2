#include "fileManager.h"
#include <string.h>
#include <stdlib.h>

const char* changeExtension(const char* path, const char* ext){
    char* d = strrchr(path, '.');
    char* newStr = (char*)malloc(sizeof(char) * (strlen(path) + strlen(ext)));

    if(d != NULL){
        strncpy(newStr, path, d - path);
        newStr[d - path] = '\0';
    }
    else{
        strcpy(newStr, path);
    }

    return (const char*)strcat(newStr, ext);
}

const char* strcatcat(const char* cat1, const char* cat2){
    char* resultCat = (char*)malloc(sizeof(char) * (strlen(cat1 + strlen(cat2))));

    strcpy(resultCat, cat1);
    strcat(resultCat, cat2);

    return (const char*)resultCat;
}
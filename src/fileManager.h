#ifndef __fileManagerH__
#define __fileManagerH__


// Muda a extensão de um arquivo, exemplo: "exemplo.png" -> "exemplo.jpeg"
// const char* novoNome = changeExtension(antigoNome, ".jpeg");
const char* changeExtension(const char* path, const char* ext);

const char* strcatcat(const char* cat1, const char* cat2);

#endif
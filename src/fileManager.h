#ifndef __fileManagerH__
#define __fileManagerH__

/**
 * @brief Altera a extensão de um arquivo.
 * 
 * @param path String original com um caractere '.' presente.
 * @param ext Nova extensão que será aplicada.
 * @return Retorna uma nova string com a extensão de path atualizado com a extensão do tipo ext.
 */
const char* changeExtension(const char* path, const char* ext);

/**
 * @brief Remove os últimos n caracteres de uma string.
 * 
 * @param string String original que será truncada.
 * @param n Número de caracteres a serem removidos do final da string.
 * @return Retorna uma nova string com os últimos n caracteres removidos.
 */
const char* trimString(const char* string, int n);

/**
 * @brief Concatena duas strings.
 * 
 * @param cat1 Primeira string.
 * @param cat2 Segunda string.
 * @return Retorna uma nova string resultante da concatenação de cat1 e cat2, respectivamente.
 */
const char* strcatcat(const char* cat1, const char* cat2);

#endif
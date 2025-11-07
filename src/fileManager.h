#ifndef __fileManagerH__
#define __fileManagerH__

#include <stdbool.h>

/**
 * @brief Altera a extensao de um arquivo.
 * @param path String original com um caractere '.' presente.
 * @param ext Nova extensao que sera' aplicada no path.
 * @return Retorna uma nova string com a extensao de path atualizado com a extensao do tipo ext.
 */
const char* changeExtension(const char* path, const char* ext);

/**
 * @brief Remove os u'ltimos n caracteres de uma string.
 * @param string String original que sera' truncada.
 * @param n Nu'mero de caracteres a serem removidos do final da string.
 * @return Retorna uma nova string com os u'ltimos n caracteres removidos.
 */
const char* trimString(const char* string, int n);

/**
 * @brief Concatena duas strings em uma outra string.
 * @param cat1 Primeira string 'a ser concatenada.
 * @param cat2 Segunda string 'a ser concatenada.
 * @return Retorna uma nova string resultante da concatenação de cat1 e cat2, respectivamente.
 */
const char* strcatcat(const char* cat1, const char* cat2);

/**
 * @brief Checa se a alocacao foi feita corretamente.
 * @param var Varia'vel 'a ser inspecionada.
 * @param text Mensagem de erro caso ocorreu falha na alocacao.
 * @return Retorna um valor booleano: True, caso houve um erro. False, caso contra'rio.
 */
bool checkAllocation(void* var, const char* text);

#endif
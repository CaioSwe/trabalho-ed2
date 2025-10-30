#ifndef _Lista_h_
#define _Lista_h_

#include <stdbool.h>
#include <stdlib.h>

/**
 * Cabeçalho dedicado à estrura "Lista" que têm como princípio
 * a inserção e remoção de qualquer elemento em qualquer posição
 * dentro da estrutura.
*/

typedef void* Lista;
typedef void* Item;

/**
 * @brief Compara dois itens e verifica sua igualdade.
 * @return Retorna um booleano indicando a igualdade dos itens comparados.
 */
typedef bool (*compararItens)(Item itemO, Item itemC);

/**
 * @brief Função de mapeamento que recebe um item e aplica uma operação sob ele.
 * @return Retorna o ponteiro para o item modificado.
 */
typedef Item (*mapFunction)(Item item);

/**
 * @brief Função que recebe um item e aplica uma operação qualquer sob ele sem modificação.
 * @return Não há retorno de algum valor.
 */
typedef void (*runThroughItems)(Item item, void* extra);

/**
 * @brief Cria uma lista genérica.
 * @return Retorna a lista criada caso houve sucesso na criação, NULL caso contrário.
 */
Lista criaLista();

/**
 * @brief Insere no início da lista.
 * @param lista Lista genérica escolhida para inserção.
 * @param item Ponteiro para um tipo qualquer de estrutura/variável na qual será inserida na lista.
 * @return Não há retorno de algum valor.
 */
void inserirInicio(Lista lista, Item item);

//void inserirMeio(Lista, Item, Item);

/**
 * @brief Insere no fim da lista.
 * @param lista Lista genérica escolhida para inserção.
 * @param item Ponteiro para um tipo qualquer de estrutura/variável na qual será inserida na lista.
 * @return Não há retorno de algum valor.
 */
void inserirFim(Lista lista, Item item);

/**
 * @brief Remove o primeiro item da lista.
 * @param lista Lista genérica escolhida para remoção do item.
 * @return Retorna o primeiro item da lista caso encontrado, NULL caso contrário.
 */
Item removerInicio(Lista lista);

//Item removerMeio(Lista, Item);

/**
 * @brief Remove o último item da lista.
 * @param lista Lista genérica escolhida para remoção do item.
 * @return Retorna o último item da lista caso encontrado, NULL caso contrário.
 */
Item removerFim(Lista lista);

/**
 * @brief Remove o item especfiicado da lista.
 * @param lista Lista genérica escolhida para remoção do item.
 * @param compFunc Função de comparação entre dois itens.
 * @param item Ponteiro para o item que será removido da lista.
 * @return Retorna o item removido caso encontrado, NULL caso contrário.
 */
Item remover(Lista lista, compararItens compFunc, Item item);

/**
 * @brief Verifica de a lista especificada está vazia.
 * @param lista Lista genérica para verificação.
 * @return Retorna Verdadeiro (True) caso a lista esteja vazia, Falso (False) caso contrário.
 */
bool isListaVazia(Lista lista);

/**
 * @brief Verifica o tamanho da lista especificada.
 * @param lista Lista genérica para verificação.
 * @return Retorna o tamanho da lista.
 */
int listaTamanho(Lista lista);

/**
 * @brief Mapeia os itens de uma lista para uma outra depois de aplicada uma dada função.
 * @param from Lista genérica da origem dos itens de mapeamento.
 * @param to Lista genérica de destino dos itens de mapeamento.
 * @param mapFunc Ponteiro de função que aplica uma operação qualquer nos itens.
 * @return Não há retorno de algum valor.
 */
void mapTo(Lista from, Lista to, mapFunction mapFunc);

/**
 * @brief Concatena duas listas.
 * @param receive Lista genérica que conterá os itens de ambas as listas.
 * @param concatFrom Lista genérica que será concatenada à lista receive.
 * @param itemSize Tamanho em bytes de um item armazenado na lista concatFrom.
 * @return Não há retorno de algum valor.
 */
void concatLista(Lista receive, Lista concatFrom, size_t itemSize);

/**
 * @brief Percorre a lista fornecida e aplica uma dada função à cada item da lista.
 * @param lista Lista genérica para percorrer.
 * @param runFunc Ponteiro de uma função qualquer que recebe cada um dos itens da lista.
 * @param extra Ponteiro para um valor/estrutura qualquer usado para auxiliar o processamento da função runFunc.
 * @return Não há retorno de algum valor.
 */
void percorrerLista(Lista lista, runThroughItems runFunc, void* extra);

/**
 * @brief Verifica se um dado elemento está contido na lista fornecida.
 * @param lista Lista genérica para verificação.
 * @param compFunc Ponteiro de uma função de comparação de dois itens.
 * @param item Item usado para comparar se ele está contido na lista.
 * @return Retorna Verdadeiro (True) caso o item esteja na lista, Falso (False) caso contrário.
 */
bool isInLista(Lista lista, compararItens compFunc, Item item);

/**
 * @brief Percorre a lista fornecida até o index do dado elemento.
 * @param lista Lista genérica para percorrer.
 * @param index Inteiro usado para indicar a posição do item.
 * @return Retorna o item posicionado no index fornecido.
 */
Item getItemLista(Lista lista, int index);

// Fazer ficar mais claro a função

/**
 * @brief Percorre a lista fornecida até o index do dado elemento.
 * @param lista Lista genérica para percorrer.
 * @param index Inteiro usado para indicar a posição do item.
 * @return Retorna o item posicionado no index fornecido.
 */
Item getItemListaI(Lista lista, compararItens compFunc, Item item);

/**
 * @brief Limpa todos os elementos da lista especificada.
 * @param lista Lista genérica para limpeza.
 * @param limparItens Valor booleano: Verdadeiro (True) para liberar (free) itens da lista, Falso (False) caso não.
 * @return Não há retorno de algum valor.
 */
void limparLista(Lista lista, bool limparItens);

/**
 * @brief Copia os itens de uma lista para uma outra lista.
 * @param from Lista genérica para cópia dos itens.
 * @param to Lista genérica para destino das cópias da lista from.
 * @param itemSize Tamanho em bytes dos itens da lista from.
 * @return Não há retorno de algum valor.
 */
void copyLista(Lista from, Lista to, size_t itemSize);

#endif
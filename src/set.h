#ifndef __SET__
#define __SET__

#include "generic.h"

typedef struct set *SET;

/**
 * Inicia um conjunto de dados.
 * @param capacity Capacidade do conjunto de dados
 * @param free Função responsável por libertar o conteúdo de cada elemento
 * @return Conjunto de dados
 */
SET initSet(int capacity, free_t free);

/**
 * Insere um novo elemento no conjunto de dados.
 * @param list Conjunto de dados onde o elemento será inserido
 * @param hash Identificador do elemento a ser inserido
 * @param content Conteúdo do elemento a ser inserido
 * @return Conjunto de dados com o novo elemento
 */
SET insertElement(SET list, char* hash, void* content);

/**
 * Copia um elemento de um conjunto de dados para outro
 * @param dest Conjunto de dados para onde o elemento será copiado
 * @param src Conjunto de dados de onde o elemento será copiado
 * @param pos Posição do elemento no conjunto de dados src
 */
SET datacpy(SET dest, SET src, int pos);

/**
 * Devolve uma cópia da hash de um elemento.
 * @param s Conjunto de dados
 * @param pos Posição onde se encontra o elemento
 * @return Cópia da hash do elemento
 */
char* getSetHash(SET s, int pos);

/**
 * Devolve o conteúdo de um elemento.
 * @param s Conjunto de dados
 * @param pos Posição onde se encontra o elemento
 * @return Conteúdo do elemento
 */
void* getSetData(SET s, int pos);

/**
 * Calcula o número de elementos que existe num conjunto de dados.
 */
int getSetSize(SET s);

/**
 * Ordena um cojunto de dados alfabeticamente a partir da hash de cada elemento
 */
void sortSetByName(SET list);

/**
 * Ordena um conjunto de dados a partir do conteúdo de cada elemento.
 * @param list Conjunto de dados
 * @param comparator Função que compara o conteúdo de dois elementos
 * @param arg Argumento opcional ao comparator
 */
void sortSet(SET list, compare_t comparator, void* arg);

/**
 * A partir de dois sets ordenados alfabeticamente, cria um novo set com todos os
 * elementos. O set resultante também se encontrará ordenado.
 */
SET unionSets(SET s1, SET s2);

/**
 * A partir de dois sets ordenados alfabeticamente, cria um novo set com os elementos
 * que apenas existem num dos sets. O set resultante também se encontrará ordenado.
 */
SET diffSets(SET s1, SET s2);

/**
 * A partir de dois sets ordenados alfabeticamente, cria um novo set com os elementos
 * comuns aos dois sets. O set resultante também se encontrará ordenado.
 */
SET intersectSet(SET s1, SET s2);

/**
 * Liberta toda a memória associada a um conjunto de dados. Se o set tiver sido 
 * inicializado com uma função free válida, o conteúdo de cada elemente será também
 * libertado.
 */
void freeSet(SET s);

#endif

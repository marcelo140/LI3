#ifndef __CATALOG__
#define __CATALOG__

#include "generic.h"
#include "set.h"

typedef struct catalog      *CATALOG;

/**
 * Inicia um catálogo com o tamanho e funções auxiliares dadas.
 * @param n Número de indíces que o catálogo terá
 * @param init Inicia o conteúdo de um elemento quando necessário
 * @param clone Função capaz de clonar o conteúdo de um elemento do catálogo
 * @param free Responsável por libertar a memória ocupado pelo conteúdo de um elemento
 */
CATALOG initCatalog (int n, init_t init, clone_t clone, free_t free);

/**
 * Altera as operações com que o catálogo foi inicializado.
 * @param cat Catálogo cujas operações serão mudadas
 * @param init Inicia o conteúdo de um elemento quando necessário
 * @param clone Função capaz de clonar o conteúdo de um elemento do catálogo
 * @param free Responsável por libertar a memória ocupado pelo conteúdo de um elemento
 */
CATALOG changeCatalogOps (CATALOG cat, init_t init, clone_t clone, free_t free);

/** 
 * Insere no índice especificado do catálogo um elemento com a hash e o conteúdo dados.
 * Se o nodo já existir, nada faz.
 * @param cat Catálogo onde será inserido o novo elemento
 * @param index Índice do catálogo onde o elemento deverá ser inserido
 * @param hash String que identifica o elemento
 * @param content Conteúdo a ser colocado no elemento
 * @result Catálogo com o elemento adicionado
 */
CATALOG insertCatalog (CATALOG cat, int index, char* hash, void* content);

/**
 * Substitui o conteúdo do elemento com a hash dada pelo novo conteúdo. Se o elemento não
 * existir, nada faz.
 * @param cat Catálogo cujo elemento será alterado
 * @param index Índice onde se encontra o elemento a ser alterado
 * @param hash String que identifica o elemento
 * @param content Novo conteúdo a ser inserido no elemento
 * @result Conteúdo antigo que se encontrava no elemento
 */
void* replaceCatalog (CATALOG cat, int index, char* hash, void* content);

/**
 * Clona o catálogo dado, incluindo as suas operações. Se a função auxiliar clone existir,
 * o conteúdo dos elementos é também clonado.
 */
CATALOG cloneCatalog (CATALOG cat);

/**
 * Devolve o conteúdo do elemento com a hash dada. Se o nodo não tiver conteúdo mas
 * existir uma função init, o conteúdo é inicializado antes de ser devolvido.
 * @param cat Catálogo a ser pesquisado
 * @param index Índice do catálogo onde se encontra o elemento pretendido
 * @param hash Indentificador do elemento
 * @return Conteúdo do elemento
 */
void* getCatContent (CATALOG cat, int index, char* hash);

/**
 * Adiciona um elemento com a hash dada à árvore, inicializando o conteúdo com a função
 * init (requerida) existente nas operações. Se o elemento já existir apenas devolve o
 * conteúdo, inicializando-o se necessário.
 * @param cat Catálogo onde se pretendo inserir o elemento
 * @param index Índice do catálogo onde se pretente inserir o elemento
 * @param hash Identificador do elemento que se pretende inserir
 * @return Conteúdo do nodo com a hash dada
 */
void* addCatalog (CATALOG cat, int index, char *hash);

/**
 * Verifica se existe um nodo com o identificador hash.
 * @param cat Catálogo a ser pesquisado.
 * @param index Índice do catálogo a ser pesquisado
 * @param hash Identificador do elemento procurado
 */
bool lookUpCatalog (CATALOG cat, int index, char* hash);

/**
 * Calcula o número de elementos existentes em todo o catálogo.
 */
int countAllElems (CATALOG cat);

/**
 * Calcula o número de elementos existentes num nodo do catálogo.
 */
int countPosElems (CATALOG cat, int index);

/**
 * Liberta o espaço ocupado por todos os elementos do catálogo. Se existir uma função free
 * no conjunto de operações do catálogo, o conteúdo de cada elemento é também libertado.
 */
void freeCatalog (CATALOG cat);

/**
 * Adiciona a um conjunto de dados todos os elementos existentes num dado índice do
 * catálogo.
 * @param cat Catálogo com os elementos pretendidos
 * @param cs Conjunto de dados onde serão inseridos os elementos pretendidos
 * @param index Índice do catálogo cujos elementos serão inseridos
 * @return Conjunto de dados com os novos elementos adicionados
 */
SET fillSet (CATALOG cat, int index);

/**
 * Adiciona a um conjunto de dados todos os elementos existentes num catálogo.
 * @param cat Catálogo com os elementos pretendidos
 * @param cs Conjunto de dados onde serão inseridos os elementos pretendidos
 * @return Conjunto de dados com os novos elementos adicionados
 */
SET fillAllSet (CATALOG cat);

/**
 * Cria um conjunto de dados com todos os elementos do catálogo para os quais a condição
 * dada é verdadeira.
 *
 * O primeiro argumento da condição será sempre o conteúdo do elemento, sendo possível
 * passar-lhe um argumento adicional.
 *
 * @param cat Catálogo a ser filtrado.
 * @param condition Condição aplicada ao conteúdo de cada elemento
 * @param arg Argumento adiciona para a condição
 * @return Conjunto de dados com os elementos filtrados
 */
SET filterCat(CATALOG cat, condition_t condition, void* arg);

/**
 * Constrói dois conjuntos de dados a partir do resultado da função comparação dada que
 * será aplicada a todos os elementos do catálogo, por ordem crescente.
 *
 * O primeiro argumento do comparador deve ser sempre o conteúdo, sendo possível
 * passar-lhe um argumento extra.
 *
 * Caso o resultado da comparação seja inferior a zero, o elemento é acrescentado ao set1.
 * Caso o resultado da comparação seja superior a zero, o elemento é acrescentado ao set2.
 * Caso o resultado da comparação seja igual a zero, o elemento é acrescentado aos dois.
 *
 * @param cat Catálogo onde será aplicado o separate
 * @param compare Função usada para identificar o set onde será acrescentado o elemento
 * @param arg Argumento opcional para o comparador
 * @param set1 Conjunto de dados onde serão adicionados elementos
 * @param set2 Conjunto de dados onde serão adicionados elementos
 */
void separateCat(CATALOG cat, compare_t compare, void* arg, SET set1, SET set2);

/**
 * Extrai o conteúdo de cada elemento, usando a função dumper dada. O conteúdo extraido será
 * guardado na estrutura data fornecida.
 * 
 * @param cat Catálogo com a informação a ser extraida
 * @param data Local onde a informação será guardada
 * @param dumper Função responsável por extrair o conteúdo dos elementos. O primeiro
 * argumento corresponde à estrutura de dados onde vai ser guardada a informação (data).
 * O segundo argumento corresponde à estrutura presente no conteúdo de cada elemento
 * @return Estrutura de dados com toda a informação
 */
void* dumpDataCat(CATALOG cat, void* data, void* (*dumper)(void*, void*));

/**
 * Constrói dois conjuntos de dados a partir do resultado da função comparação. A função
 * comparação apenas é aplicada a elementos para o qual a condição dada é verdade. 
 * Os elementos em que a condição é falsa são ignorados.
 *
 * O primeiro argumento da condição e do comparador deve corresponder sempre ao conteúdo
 * do nodo. Cada uma destas funçãos pode receber um argumento opcional.
 
 * Caso o resultado da comparação seja inferior a zero, o nodo é acrescentado ao set1.
 * Caso o resultado da comparação seja superior a zero, o nodo é acrescentado ao set2.
 * Caso o resultado da comparação seja igual a zero, o nodo é acrescentado aos dois sets.
 *
 * @param cat Catálogo onde será aplicado a função
 * @param set1 Conjunto de dados onde serão adicionados elementos
 * @param set2 Conjunto de dados onde serão adicionados elementos
 * @param condition Condição aplicada ao conteúdo de cada nodo
 * @param cond_arg Argumento opcional para a condição
 * @param comparator Função usada para indentificar o conjunto de dados onde será 
 * acrescentado o elemento
 * @param comp_arg Argumento opcional para a condição
 */
void condSeparateCat(CATALOG cat, SET set1, SET set2,
                                  condition_t condition, void* cond_arg,
                                  compare_t   comparator, void* comp_arg);


#endif

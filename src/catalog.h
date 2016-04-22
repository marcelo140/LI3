#ifndef __CATALOG__
#define __CATALOG__

#include "generic.h"
#include "set.h"

typedef struct catalog *CATALOG;

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
 * @param set Conjunto de dados onde serão inseridos os elementos pretendidos
 * @param index Índice do catálogo cujos elementos serão inseridos
 * @return Conjunto de dados com os novos elementos adicionados
 */
SET fillSet (CATALOG cat, SET set, int index);

/**
 * Adiciona a um conjunto de dados todos os elementos existentes num catálogo.
 * @param cat Catálogo com os elementos pretendidos
 * @param set Conjunto de dados onde serão inseridos os elementos pretendidos
 * @return Conjunto de dados com os novos elementos adicionados
 */
SET fillAllSet (CATALOG cat, SET set);

/**
 * Cria um conjunto de dados com todos os elementos do catálogo para os quais a condição
 * dada é verdadeira.
 *
 * O primeiro argumento da condição será sempre o conteúdo do elemento, sendo possível
 * passar-lhe um argumento adicional.
 *
 * @param cat Catálogo a ser filtrado
 * @param set Conjunto de dados onde serão adicionados os novos elementos
 * @param condition Condição aplicada ao conteúdo de cada elemento
 * @param arg Argumento adiciona para a condição
 * @return Conjunto de dados com os elementos filtrados
 */
SET filterCat(CATALOG cat, SET set, condition_t condition, void* arg);

/**
 * Transforma o conteúdo de cada elemento, usando a função dumper dada. O resultado do
 * dumper é então adicionado, juntamente com a hash do elemento, ao set dado.
 * 
 * @param cat Catálogo com a informação a ser extraida
 * @param data Local onde a informação será guardada
 * @param dumper Função responsável por extrair o conteúdo dos elementos. O primeiro
 * argumento corresponde à estrutura de dados onde vai ser guardada a informação (data).
 * O segundo argumento corresponde à estrutura presente no conteúdo de cada elemento
 * @return Estrutura de dados com toda a informação
 */
SET dumpCatalog(CATALOG cat, SET set, void* (*dumper)(void*));

#endif

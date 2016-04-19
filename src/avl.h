#ifndef __AVL__
#define __AVL__

#include "generic.h"
#include "set.h"

typedef struct avl      *AVL;

/**
 * Inicia uma AVL com as funções auxiliares dadas.
 * @param init Inicia o conteúdo quando necessário
 * @param equals Verifica se dois conteúdos são iguais
 * @param clone Função capaz de clonar o conteúdo de um element da árvore
 * @param free Responsável por libertar toda a memória ocupada pelo conteúdo de um elemento
 */
AVL initAVL (init_t init, condition_t equals, clone_t clone, free_t free);

/**
 * Altera as operações com que a árvore foi inicializada.
 * @param tree Árvore cujas operações serão mudadas
 * @param init Inicia o conteúdo quando necessário
 * @param equals Verifica se dois conteúdos são iguais
 * @param clone Função capaz de clonar o conteúdo de uma árvore
 * @param free Responsável por libertar toda a memória ocupada pelo conteúdo
 */
AVL changeOps (AVL tree, init_t init, condition_t equals, clone_t clone, free_t free);

/**
 * Insere na árvore um nodo com a hash e o conteúdo dados. Se o nodo já existe, nada faz.
 * @param tree Árvore onde vai ser inserido o nodo
 * @param hash String que identifica o nodo
 * @param content Conteúdo a ser colocado no nodo
 * @result Árvore onde o nodo foi inserido
 */
AVL insertAVL (AVL tree, char *hash, void *content);

/**
 * Substitui o conteúdo do nodo com a hash dada pelo novo conteúdo. Se o nodo não existir,
 * nada faz.
 * @param tree Árvore com o nodo a ser alterado
 * @param hash Identificador do nodo
 * @param content Novo conteúdo a ser inserido no nodo
 * @return Conteúdo antigo que se encontrava no nodo
 */
void* replaceAVL (AVL tree, char *hash, void *content);

/**
 * Clona a árvore dada, incluindo as suas operações. Se a função auxiliar clone existir,
 * o conteúdo dos elementos é também clonado.
 */
AVL cloneAVL (AVL tree);

/** 
 * Devolve o conteúdo do nodo com a hash dada. Se o nodo não tiver conteúdo mas existir
 * uma função init, o conteúdo é inicializado antes de ser devolvido.
 */
void* getAVLcontent (AVL tree, char *hash);

/**
 * Adiciona um nodo com a hash dada à árvore, inicializando o conteúdo com a função init
 * (requerida) existente nas operações. Se o nodo já existir apenas devolve o conteúdo,
 * inicializando-o se necessário.
 * @param tree Árvore onde se pretende inserir o elemento
 * @param hash Identificador do nodo
 * @return Conteúdo do nodo com a hash dada
 */ 
void* addAVL (AVL tree, char *hash);

/**
 * Verifica se existe um nodo com o identificador hash.
 * @param tree Árvore a ser verificada
 * @param hash Identificador do elemento procurado
 */
bool lookUpAVL (AVL tree, char *hash);

/**
 * Verifica se duas árvores são iguais. Duas árvores são iguais se tiverem as mesmas
 * operações e os mesmos nodos. Se existir uma operação equals no set de operações
 * da árvore, é também verificado se o conteúdo de cada nodo de cada árvore é igual.
 */
bool equalsAVL (AVL tree, AVL b);

/**
 * Verifica se a árvore é, ou não, vazia.
 */
bool isEmptyAVL (AVL tree);

/*
 * Determina o número de nodos existentes na árvore.
 */
int countNodes (AVL tree);

/**
 * Liberta o espaço ocupado pelos nodos da árvore. Se existir uma função free no conjunto de
 * operações da árvore, o conteúdo de cada nodo é também libertado.
 */
void freeAVL (AVL n);

SET addAVLtoSet (SET s, AVL tree);

SET filterAVL (AVL tree, SET ds, condition_t condition, void* arg);

void separateAVL (AVL tree, SET set1, SET set2, compare_t compare, void* arg);

void* dumpDataAVL(AVL tree, void* data, void* (*dumper)(void*, void*));

void condSeparateAVL (AVL tree, SET set1, SET set2,
                      condition_t condition,  void* cond_arg,
                      compare_t   comparator, void* comp_arg);
               
#endif

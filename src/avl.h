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

/**
 * Adiciona os elementos de uma AVL ao set dado. O conteúdo dos nodos apenas é copiado
 * caso exista uma função clone no set de funções da árvore.
 * @param s Set onde serão adicionados os elementos
 * @param tree Árvore cujos elementos serão adicionados
 * @return Set s, agora com os elementos adicionados
 */
SET addAVLtoSet (SET s, AVL tree);

/**
 * Adiciona ao conjunto de dados indicado uma cópia de todos os elementos da árvore
 * para a qual a condição dada é verdadeira.
 * @param tree Árvore cujos nodos serão filtrados
 * @param s Conjunto de dados onde serão adicionados os novos elementos
 * @param condition Condição a que cada nodo será submetido
 * @param arg Argumento opcional para a condição
 * @return Set s, agora com os elementos adicionados
 */
SET filterAVL (AVL tree, SET s, condition_t condition, void* arg);

/**
 ** Constrói dois conjuntos de dados a partir do resultado da função comparação dada que
 ** será aplicada a todos os nodos da árvore, por ordem crescente.
 **
 ** O primeiro argumento do comparador deve ser sempre o conteúdo, sendo possível 
 ** passar-lhe um argumento extra.
 **
 ** Caso o resultado da comparação seja inferior a zero, o nodo é acrescentado ao set1.
 ** Caso o resultado da comparação seja superior a zero, o nodo é acrescentado ao set2.
 ** Caso o resultado da comparação seja igual a zero, o nodo é acrescentado aos dois sets.
 **
 ** @param tree Árvore onde será aplicado o separate
 ** @param set1 Conjunto de dados onde serão adicionados elementos
 ** @param set2 Conjunto de dados onde serão adicionados elementos
 ** @param compare Função usada para indentificar o conjunto de dados onde será acrescentao
 ** o nodo
 ** @param arg Argumento opcional para o comparador
 **/
void separateAVL (AVL tree, SET set1, SET set2, compare_t compare, void* arg);

/**
 ** Extrai o conteúdo de cada nodo, usando a função dumper dada. O conteúdo extraido será
 ** guardado na estrutura data fornecida.
 ** 
 ** @param tree Árvore com a informação a ser extraida
 ** @param data Local onde a informação será guardada
 ** @param dumper Função responsável por extrair o conteúdo dos nodos. O primeiro argumento
 ** corresponde à estrutura de dados onde vai ser guardada a informação (data). O segundo
 ** argumento corresponde à estrutura de dados presente no conteúdo de cada nodo
 ** @return Estrutura de dados com toda a informação
 **/
void* dumpDataAVL(AVL tree, void* data, void* (*dumper)(void*, void*));

/**
 ** Constrói dois conjuntos de dados a partir do resultado da função comparação. A função
 ** comparação apenas é aplicada a nodos para o qual a condição dada é verdade. Os nodos
 ** em que a condição é falsa são ignorados.
 **
 ** O primeiro argumento da condição e do comparador deve corresponder sempre ao conteúdo
 ** do nodo. Cada uma destas funçãos pode receber um argumento opcional.
 *
 ** Caso o resultado da comparação seja inferior a zero, o nodo é acrescentado ao set1.
 ** Caso o resultado da comparação seja superior a zero, o nodo é acrescentado ao set2.
 ** Caso o resultado da comparação seja igual a zero, o nodo é acrescentado aos dois sets.
 **
 ** @param tree Árvore onde será aplicado a função
 ** @param set1 Conjunto de dados onde serão adicionados elementos
 ** @param set2 Conjunto de dados onde serão adicionados elementos
 ** @param condition Condição aplicada ao conteúdo de cada nodo
 ** @param cond_arg Argumento opcional para a condição
 ** @param comparator Função usada para indentificar o conjunto de dados onde 
 ** será acrescentao o nodo
 ** @param comp_arg Argumento opcional para a condição
 **/
void condSeparateAVL (AVL tree, SET set1, SET set2,
                      condition_t condition,  void* cond_arg,
                      compare_t   comparator, void* comp_arg);
               
#endif

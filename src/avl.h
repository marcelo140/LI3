#ifndef __AVL__
#define __AVL__

#include "generic.h"

typedef struct avl      *AVL;
typedef struct data_set *DATASET;

/**
 * Inicia uma AVL com as funções auxiliares dadas.
 * @param init Inicia o conteúdo quando necessário
 * @param equals Verifica se dois conteúdos são iguais
 * @param clone Função capaz de clonar o conteúdo de uma árvore
 * @param free Responsável por libertar toda a memória ocupada pelo conteúdo
 */
AVL initAVL (init_t init, condition_t equals, clone_t clone, free_t free);

/**
 * Altera as operações com que a árvore foi inicializada para as funções dadas.
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
 * o conteúdo é também clonado.
 */
AVL cloneAVL (AVL tree);

/** 
 * Devolve o conteúdo do nodo com a hash dada. Se o nodo não tiver conteúdo mas existir
 * uma função init, o conteúdo é inicializado antes de ser devolvido.
 */
void*  getAVLcontent (AVL tree, char *hash);

/**
 * Adiciona um nodo com a hash dada à árvore, inicializando o conteúdo com a função init
 * (requerida) existente nas operações. Se o nodo já existir apenas devolve o conteúdo,
 * inicializando-o se necessário.
 * @param tree Árvore a ser procurada
 * @param hash Identificador do nodo
 * @return Conteúdo do nodo com a hash dada
 */ 
void* addAVL (AVL tree, char *hash);

/**
 * Verifica se existe um nodo com o identificador hash.
 * @param tree Árvore a ser verificada
 * @param hash Identificador dado
 * @return Retorna true se existir, false caso contrário
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
 * Liberta o espaço ocupado pelos nodos da árvore. Se existir uma função free no set de
 * operações da árvore, o conteúdo de cada nodo é também libertado.
 */
void freeAVL (AVL n);

/**
 * Inicia um conjunto de dados com o tamanho dado.
 * @param n Tamanho inicial do conjunto de dados
 */
DATASET initDataSet (int n);

/**
 * Adiciona informação sobre todos os nodos da árvore ao conjunto de dados. Os nodos
 * são adicionados por ordem crescente.
 * @param ds Conjunto de dados onde será acrescentada a informação
 * @param tree Árvore com a informação que será adicionada ao conjunto de dados
 * @return Conjunto de dados com a nova informação
 */
DATASET addDataSet (DATASET ds, AVL tree);

/**
 * Copia um elemento de um conjunto de dados para outro.
 * @param dest Conjunto de dados para onde será copiado o novo elemento
 * @param source Conjundo de dados de onde é copiado o elemento
 * @param pos Posição do elemento no conjunto de dados fonte
 * @return Conjunto de dados com o novo elemento
 */
DATASET datacpy (DATASET dest, DATASET source, int pos);

/**
 * Adiciona ao conjunto de dados todos os elementos da AVL para os quais a condição dada
 * é verdadeira. 
 *
 * O primeiro argumento da condição será sempre o conteúdo da nodo, sendo possível
 * passar-lhe um argumento adicional.
 *
 * @param tree Árvore a ser filtrada
 * @param ds Conjunto de dados onde serão adicionados os novos elementos
 * @param condition Condição aplicada ao conteúdo de cada nodo
 * @param arg Argumento adicional para a condição
 * @return Conjunto de dados ao qual foi acrescentado os novos elementos
 */
DATASET filterAVL (AVL tree, DATASET ds, condition_t condition, void* arg);

/**
 * Constrói dois conjuntos de dados a partir do resultado da função comparação dada que
 * será aplicada a todos os nodos da árvore, por ordem crescente.
 *
 * O primeiro argumento do comparador deve ser sempre o conteúdo, sendo possível 
 * passar-lhe um argumento extra.
 *
 * Caso o resultado da comparação seja inferior a zero, o nodo é acrescentado ao set1.
 * Caso o resultado da comparação seja superior a zero, o nodo é acrescentado ao set2.
 * Caso o resultado da comparação seja igual a zero, o nodo é acrescentado aos dois sets.
 *
 * @param tree Árvore onde será aplicado o separate
 * @param compare Função usada para indentificar o conjunto de dados onde será acrescentao
 * o nodo
 * @param arg Argumento opcional para o comparador
 * @param set1 Conjunto de dados onde serão adicionados elementos
 * @param set2 Conjunto de dados onde serão adicionados elementos
 */
void separateAVL (AVL tree, DATASET set1, DATASET set2, compare_t compare, void* arg);

/**
 * Extrai o conteúdo de cada nodo, usando a função dumper dada. O conteúdo extraido será
 * guardado na estrutura data fornecida.
 * 
 * @param tree Árvore com a informação a ser extraida
 * @param data Local onde a informação será guardada
 * @param dumper Função responsável por extrair o conteúdo dos nodos. O primeiro argumento
 * corresponde à estrutura de dados onde vai ser guardada a informação (data). O segundo
 * argumento corresponde à estrutura de dados presente no conteúdo de cada nodo
 * @return Estrutura de dados com toda a informação
 */
void* dumpDataAVL(AVL tree, void* data, void* (*dumper)(void*, void*));

/**
 * Constrói dois conjuntos de dados a partir do resultado da função comparação. A função
 * comparação apenas é aplicada a nodos para o qual a condição dada é verdade. Os nodos
 * em que a condição é falsa são ignorados.
 *
 * O primeiro argumento da condição e do comparador deve corresponder sempre ao conteúdo
 * do nodo. Cada uma destas funçãos pode receber um argumento opcional.
 
 * Caso o resultado da comparação seja inferior a zero, o nodo é acrescentado ao set1.
 * Caso o resultado da comparação seja superior a zero, o nodo é acrescentado ao set2.
 * Caso o resultado da comparação seja igual a zero, o nodo é acrescentado aos dois sets.
 *
 * @param tree Árvore onde será aplicado a função
 * @param set1 Conjunto de dados onde serão adicionados elementos
 * @param set2 Conjunto de dados onde serão adicionados elementos
 * @param condition Condição aplicada ao conteúdo de cada nodo
 * @param cond_arg Argumento opcional para a condição
 * @param comparator Função usada para indentificar o conjunto de dados onde 
 * será acrescentao o nodo
 * @param comp_arg Argumento opcional para a condição
 */
void condSeparateAVL (AVL tree, DATASET set1, DATASET set2,
                                condition_t condition,  void* cond_arg,
                                compare_t   comparator, void* comp_arg);
               
/**
 * Reordena o conteúdo de um conjunto de dados usando o comparador dado.
 * @param set Conjunto de dados a ser ordenado
 * @param begin Posição a partir da qual o set deve ser ordenado
 * @param end Posição até que o set deve ser ordenado
 * @param comparator Função que define como comparar dois elementos
 */ 
void sortDataSet (DATASET set, int begin, int end, compare_t comparator);

/**
 * Concatena os dois conjuntos de dados, acrescentado os elementos do set2 ao fim do set1
 */
DATASET concatDataSet (DATASET set1, DATASET set2);

/**
 * Dado dois conjuntos previamente ordenados, une-se os dois sets, eliminando elementos
 * repetidos. O resultado é também um conjunto ordenado.
 * @param dest Conjunto que será alterado
 * @param src Conjunto cujos dados serão agregados ao conjunto dest
 * @return Apontador para o inicio do conjunto com todos os dados agregados (dest)
 */
DATASET unionDataSets (DATASET dest, DATASET src);

/**
 * Dado dois conjuntos previamente ordenados, une-se os dois sets, eliminando todos os
 * elementos em comum. O resultado é também um conjunto ordenado.
 * @param dest Conjunto que será alterado
 * @param src Conjunto cujos dados serão agregados ao conjunto dest
 * @return Apontador para o inicio do conjunto com todos os dados agregados (dest)
 */
DATASET diffDataSets (DATASET dest, DATASET src);

/**
 * Dado dois conjuntos previeamente ordenados, une-se os dois sets, mantendo apenas os
 * elementos em comum. O resultado será também um set ordenado.
 * @return Conjunto de dados resultante
 */
DATASET intersectDataSet (DATASET d1, DATASET d2);

/**
 * Devolve o conteúdo do elemento na posição indicada. Este conteúdo corresponde ao 
 * existente na árvore no nodo com o mesmo identificador.  Se a posição não for válida é
 * devolvido NULL.
 */
void* getDataPos (DATASET ds, int pos);

/** 
 * Devolve uma cópia do identificado do elemento na posição indicada. Se a posição não
 * for válida é devolvido NULL.
 */
char* getHashPos (DATASET ds, int pos);

/**
 * Determina o tamanho de um conjunto de dados.
 */
int getDataSetSize (DATASET ds);

/**
 * Liberta o espaço ocupado pelo conjunto de dados dado.
 */
void freeDataSet (DATASET set);

#endif

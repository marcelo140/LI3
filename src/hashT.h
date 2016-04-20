#ifndef __HASHT__
#define __HASHT__

#include "generic.h"
#include "set.h"

typedef struct hasht *HASHT;

typedef void* (*add_t)  (void*, void*);

/**
 * Inicializa uma nova tabela de Hash 
 * @return Nova tabela de Hash
 */
HASHT initHashT(init_t init, add_t add, clone_t clone, free_t free);
HASHT initMyHashT(int size,init_t init, add_t add, clone_t clone, free_t free);

/**
 * Insere um dado conteúdo com uma certa chave na Tabela de Hash.
 * Caso a chave já existe, adidiciona o conteúdo novo ao conteúdo existente.
 * @param ht Tabela de Hash onde inserir
 * @param key Chave a inserir
 * @param content Conteúdo a inserir
 * @return Tabela de Hash atualizada
 */
HASHT insertHashT(HASHT ht, char* key, void* content);

/**
 * Devolve o conteúdo de uma dada chave
 * @param ht Tabela de Hash a consultar
 * @param key Chave do conteúdo pretendido
 * @return Conteúdo da chave
 */
void* getHashTcontent(HASHT ht, char* key);

/**
 * Devolve um conjunto com todo o conteúdo da Tabela de Hash
 * @param ht Tabela de Hash onde ler
 * @return Conjunto com todo o conteúdo da Tabela
 */ 
SET dumpHashT(HASHT ht, SET set);

/**
 * Liberta a memória ocupada por uma dada Tabela de Hash
 * @param ht Tabela de Hash a libertar
 */
void freeHashT(HASHT ht);

#endif

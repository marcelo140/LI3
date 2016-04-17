#ifndef __HASHT_H__
#define __HASHT_H__

typedef struct hasht *HASHT;
typedef struct hashtSet *HASHTSET;
typedef void* (*ht_init_t) ();
typedef void* (*ht_add_t) (void*, void*);
typedef void  (*ht_free_t) (void*);

/**
 * Inicializa uma nova tabela de Hash 
 * @return Nova tabela de Hash
 */
HASHT initHashT(ht_init_t init, ht_add_t add, ht_free_t free);

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
HASHTSET dumpHashT(HASHT ht);

/**
 * Devolve o conteúdo de uma dada posição no HASHTSET
 * @param hts Conjunto onde procurar
 * @param pos Posição do conteúdo a retornar
 * @return Conteúdo da dada posição
 */
void* getHashTSetContent(HASHTSET hts, int pos);

/**
 * Devolve a chave de uma dada posição no HASHTSET
 * @param hts Conjunto onde procurar
 * @param pos Posição da chave a retornar
 * @return Chave da dada posição
 */
char* getHashTSetKey(HASHTSET hts, int pos);

/**
 * Concatena duas HashSets numa só colocando todo o h2 em h1
 * @return Concatenação dos dois HashSets
 */
HASHTSET concatHashTSet(HASHTSET h1, HASHTSET h2);

/**
 * Liberta a memória ocupada por uma dada Tabela de Hash
 * @param ht Tabela de Hash a libertar
 */
void freeHashT(HASHT ht);

#endif

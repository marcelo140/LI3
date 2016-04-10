#include <string.h>
#include <stdlib.h>
#include "hashT.h"

#define MAX_STR_SIZE 10
#define BASE_CAPACITY 31
#define CONTENT(i) ht->content[i]


typedef struct hashCntt {
	HASH hash;
	void *back;
	REVENUE revenue;
} HashCntt;

struct table {
	int	capacity;	
	HashCntt* content;
};

struct hash {
	char *seed;
	int key;
};

static int quadraticProbing(HASHTABLE ht, int key);

/**
 * Inicia uma HASHTABLE nova
 * @return Nova HASHTABLE
 */
HASHTABLE initHashTable() {
	HASHTABLE new = malloc(sizeof (*new));
	int i;

	new->capacity = BASE_CAPACITY;
	new->content = malloc(BASE_CAPACITY * sizeof(struct hashCntt));

	for (i=0; i<BASE_CAPACITY; i++) {
		new->content[i].hash = NULL;
		new->content[i].back = NULL;
		new->content[i].revenue = initRevenue();
	}
	
	return new;
}

/**
 * Insere uma SALE e um apontador para o pai na HASHTABLE
 * Se já existe, apenas adiciona à REVENUE existente
 * @param ht HASHTABLE a atualizar
 * @param back Apontador para o pai  
 * @param s SALE a atualizar
 * @return HASHTABLE atualizada
 */
HASHTABLE insertHashT(HASHTABLE ht, void *back, SALE s) {
	int key;
	PRODUCT p;
	HASH hash;

	p = getProduct(s);
	hash = createHash(ht, fromProduct(p));
	key = hash->key;

	if (CONTENT(key).hash != NULL && !strcmp(CONTENT(key).hash->seed, hash->seed)) 
		key = quadraticProbing(ht, key);

	/* Se já existe, apenas atualiza */
	if (strcmp(CONTENT(key).hash->seed, hash->seed)) {
		CONTENT(key).back = back;
		CONTENT(key).hash = hash;
	}
	CONTENT(key).revenue = addSale(CONTENT(key).revenue, s);
		

	freeProduct(p);
	freeHash(hash);
	
	return ht;
}

/**
 * Liberta o espaço ocupado por uma HashTable
 * @param ht HASHTABLE a libertar
 */
void freeHashTable(HASHTABLE ht) {
	freeRevenue(ht->content->revenue);
	freeHash(ht->content->hash);
	free(ht->content);
	free(ht);
}

/**
 * Devolve o endereço do pai do elemento com essa hash
 * @param ht HASHTABLE a consultar
 * @param hash Hash do elemento
 * @return o endereço pai do elemento
 */
void* getParent(HASHTABLE ht, HASH hash) {

	if ( strcmp(CONTENT(hash->key).hash->seed, hash->seed)) 
		hash->key = quadraticProbing(ht, hash->key);
   	
	return CONTENT(hash->key).back;
}

/**
 * Devolve o total faturado de um dado elemento num certo mês, com ou sem promoção
 * @param ht HASHTABLE a consultar
 * @param hash HASH do elemento
 * @param month Mês a consultar
 * @param branch Filial a consultar
 * @param MODE MODE_N caso seja normal, MODE_P caso seja promoção
 * @return Total faturado do elemento no dado mes e modo
 */
double getHashBilled(HASHTABLE ht, HASH hash, int month, int branch, int MODE) {
	
	if ( strcmp(CONTENT(hash->key).hash->seed, hash->seed)) 
		hash->key = quadraticProbing(ht, hash->key);
   	
	return getBilled(CONTENT(hash->key).revenue, month, branch, MODE);
}

/**
 * Devolve a quantidade total comprada de um dado elemento, num certo mês, com ou sem promoção
 * @param ht HASHTABLE a consultar
 * @param hash HASH do elemento
 * @param month Mês a consultar
 * @param branch Filial a consultar
 * @param MODE MODE_N caso seja normal, MODE_P caso seja promoção
 * @return Quantidade total comprada de um dado elemento num dado mês e modo
 */
int getHashQuantity(HASHTABLE ht, HASH hash, int month, int branch, int MODE) {
	
	if ( strcmp(CONTENT(hash->key).hash->seed, hash->seed)) 
		hash->key = quadraticProbing(ht, hash->key);
   	
	return getQuantity(CONTENT(hash->key).revenue, month, branch, MODE);
}

/**
 * Devolve toda a REVENUE de um dado elemento
 * @param ht HASHTABLE a consultar
 * @param hash HASH do elemnto
 * @return REVENUE do elemento
 */
REVENUE getHashRevenue (HASHTABLE ht, HASH hash) {

	if ( strcmp(CONTENT(hash->key).hash->seed, hash->seed)) 
		hash->key = quadraticProbing(ht, hash->key);
   	
	return CONTENT(hash->key).revenue;
}


/**
 * Cria uma HASH de uma string para uma dada HASHTABLE
 * @param ht HASHTABLE exemplo
 * @param seed String que serve como seed
 * @return HASH criada
 */
HASH createHash(HASHTABLE ht, char *seed) {
	HASH new = malloc(sizeof(*new));
	int i;
	
	new->seed= malloc(MAX_STR_SIZE);
	strcpy (new->seed, seed);

	/* Este algoritmo de criação de hashes deve ser melhorado */
	for (i=0; seed[i] != '\0' && i < MAX_STR_SIZE; i++) {
		new->key += seed[i];
	} 

	new->key %= ht->capacity;

	return new;
}

/**
 * Liberta o espaço ocupado pela HASH
 * @param hash HASH a libertar
 */
void freeHash(HASH hash) {
	free(hash->seed);
	free(hash);
}

static int quadraticProbing(HASHTABLE ht, int key) {
	int i, pos = key + 1;

	for (i=1; pos != key || i > ht->capacity; i++) {
		if (CONTENT(pos).hash == NULL) key = pos;
		else pos = (key + i*i) % ht->capacity; 
	}
	
	return pos;
}

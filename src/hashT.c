#include <string.h>
#include <stdlib.h>
#include "hashT.h"
#include "revenue.h"

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

HASHTABLE insertHashT(HASHTABLE ht, void *back, SALE s) {
	int quantity, billed, month, mode, i, key, pos;
	PRODUCT p;
	HASH hash;

	quantity = getQuant(s);
	billed = getPrice(s) * quantity;
	month = getMonth(s);
	mode = getMode(s);	

	p = getProduct(s);
	hash = createHash(ht, fromProduct(p));
	freeProduct(p);
	key = hash->key;

	if (ht->content[key].hash != NULL && !strcmp(CONTENT(key).hash->seed, hash->seed)) {
		pos = key+1 % ht->capacity; 
		for (i=1; pos != key || i > ht->capacity; i++) {

			if (ht->content[pos].hash == NULL) key = pos;
			else pos = (key + i*i) % ht->capacity; 
		}
	}

	/* Se já existe, apenas atualiza */
	if (strcmp(CONTENT(key).hash->seed, hash->seed)) {
		ht->content[key].back = back;
		ht->content[key].hash = hash;
	}
	CONTENT(key).revenue = updateRevenue(CONTENT(key).revenue,
		   									month, mode, billed, quantity);
		

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
	return ht->content[hash->key].back;
}

/**
 * Devolve o total faturado de um dado elemento num certo mês, com ou sem promoção
 * @param ht HASHTABLE a consultar
 * @param hash HASH do elemento
 * @param month Mês a consultar
 * @param MODE MODE_N caso seja normal, MODE_P caso seja promoção
 * @return Total faturado do elemento no dado mes e modo
 */
double getHashBilled(HASHTABLE ht, HASH hash, int month, int MODE) {
	return getBilled(CONTENT(hash->key).revenue, month, MODE);
}

/**
 * Devolve a quantidade total comprada de um dado elemento, num certo mês, com ou sem promoção
 * @param ht HASHTABLE a consultar
 * @param hash HASH do elemento
 * @param month Mês a consultar
 * @param MODE MODE_N caso seja normal, MODE_P caso seja promoção
 * @return Quantidade total comprada de um dado elemento num dado mês e modo
 */
int getHashQuantity(HASHTABLE ht, HASH hash, int month, int MODE) {
	return getQuantity(CONTENT(hash->key).revenue, month, MODE);
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

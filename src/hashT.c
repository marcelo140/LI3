/*********************************
 * 	HASH TABLE AUXILIAR À HEAP   *
 *********************************/

#include <string.h>
#include <stdlib.h>
#include "hashT.h"

#define MAX_STR_SIZE 10
#define BASE_CAPACITY 31
#define CONTENT(i) ht->cntt[i]
#define EMPTY 0
#define REMOVED -1
#define USED 1

struct hashCntt {
	int position;
	char status;
	char seed[MAX_STR_SIZE];
};

struct table {
	int	capacity;	
	struct hashCntt* cntt;
};

static int hash(char *seed, int size);
static HASHTABLE resizeHashT(HASHTABLE ht);

/**
 * Inicia uma HASHTABLE nova
 * @return Nova HASHTABLE
 */
HASHTABLE initHashTable() {
	HASHTABLE new = malloc(sizeof (*new));

	new->capacity = BASE_CAPACITY;
	new->cntt = calloc(BASE_CAPACITY, sizeof(struct hashCntt));

	return new;
}

/**
 * Insere uma SALE e um apontador para o pai na HASHTABLE
 * Se já existe, apenas adiciona à REVENUE existente
 * @param ht HASHTABLE a atualizar
 * @param seed Hash a inserir
 * @param pos Respetiva posição
 * @return HASHTABLE atualizada
 */
HASHTABLE insertHashT(HASHTABLE ht, char *seed, int pos) {
	int p, n, i=1, cap = ht->capacity, loop=0;

	p = hash(seed, cap);	
	if (ht->cntt[p].status == EMPTY || ht->cntt[p].status == REMOVED) {
		ht->cntt[p].position = pos;
		strcpy(ht->cntt[p].seed, seed);
		ht->cntt[p].status = USED;
	}
	else {
		n = p;
		for(n=p; !loop && CONTENT(n).status == USED ; i++) {
			n = (p + i*i) % cap;
			if (n == p) loop = 1;	
			i++;	
		}

		if (loop) {
			ht = resizeHashT(ht);
			ht = insertHashT(ht, seed, pos);
		}
	}
	
	return ht;
}

int getPosition(HASHTABLE ht, char *seed) {
	int h, p, i;

	h = hash(seed, ht->capacity);
	
	for(p=h, i=0; strcmp(CONTENT(p).seed, seed) != 0 && i < ht->capacity; i++)
		p = (h + (i*i)) % ht->capacity;
	
	if (i >= ht->capacity) p = -1;
	
	return p;
}

/** 
 * Liberta o espaço ocupado por uma HashTable
 * @param ht HASHTABLE a libertar
 */
void freeHashTable(HASHTABLE ht) {
	free(ht->cntt);
	free(ht);
}

/**
 * Cria uma HASH de uma string para uma dada HASHTABLE
 * @param seed String que serve como seed
 * @param size Tamanho da tabela de Hash
 */
static int hash(char *seed, int size) {
	int i, r=0;

	/* Este algoritmo de criação de hashes deve ser melhorado */
	for (i=0; seed[i] != '\0' && i < MAX_STR_SIZE; i++) {
		r += seed[i];
	} 

	r %= size;

	return r;
}

static HASHTABLE resizeHashT(HASHTABLE ht) {
	int i, j, newCap;
	struct hashCntt* newCntt;

	newCap = ht->capacity * 2;
	newCntt = calloc(newCap, sizeof(struct hashCntt));

	for (i=0; i < ht->capacity; i++){
		if (CONTENT(i).status == USED){
			j = hash(CONTENT(i).seed, newCap);
			newCntt[j].position = CONTENT(i).position;
			strcpy(newCntt[j].seed, CONTENT(i).seed);
			newCntt[j].status = USED;
		} 
	}

	free(ht->cntt);
	ht->cntt = newCntt;
	
	return ht;
}



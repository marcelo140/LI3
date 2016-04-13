#include <stdlib.h>
#include <string.h>

#include "avl.h"

#define HASH_SIZE 10

typedef enum balance { LH, EH, RH } Balance;

typedef struct node {
	char *hash;
	void *content;
	struct node *left, *right;
	Balance bal;
} *NODE;

struct avl {
	NODE head;
	int size;

	void* (*init)   ();
	bool  (*equals) (void *content1, void *content2);
	void* (*clone)  (void *content);
	void  (*free)   (void *content);
};

struct dataSet {
	int size;
	int pos;
	NODE *set;
};

static NODE newNode(char *hash, void *content, NODE left, NODE right);
static NODE rotateRight(NODE node);
static NODE rotateLeft(NODE node);
static NODE balanceRight(NODE node);
static NODE balanceLeft(NODE node);
static NODE insertRight(NODE node, NODE new, int *update, NODE *last);
static NODE insertLeft(NODE node, NODE new, int *update, NODE *last);
static NODE insertNode(NODE node, NODE new, int *update, NODE* last);
static NODE cloneNode(NODE n, void* (*clone)(void *));
static bool equalsNode(NODE a, NODE b, bool (*equals)(void*, void*));
static void freeNode(NODE node, void (*freeContent)(void *));
static DATASET insertDataSet (DATASET ds, NODE n);
static DATASET addDataSetAux(DATASET ds, NODE node);

/**
 * Inicia uma nova AVL.
 * @return Nova AVL
 */
AVL initAVL(void* (*init) (), bool (*equals)(void*, void*), 
            void* (*clone)(void*), void (*free)(void *)){

	AVL tree = malloc (sizeof (struct avl));

	tree->head = NULL;
	tree->size = 0;

	tree->init = init;
	tree->equals = equals;
	tree->clone = clone;
	tree->free = free;

	return tree;
}

/**
 * Insere conteúdo na AVL com Hash característica do Nodo.
 * @param tree AVL a onde inserir
 * @param s Hash a inserir
 * @param c Conteúdo a inserir
 * @return AVL com o novo nodo.
 */
AVL insertAVL(AVL tree, char *hash, void *content) {
	int update = 0;
	NODE new, last;

	new = newNode(hash, content, NULL, NULL);

	tree->head = insertNode(tree->head, new, &update, &last);
	(last == new) ? tree->size++ : free(new);

	return tree;
}

/**
 * Atualiza o conteúdo de um nodo da AVL caracterizado por uma Hash
 * @param tree AVL
 * @param hsh Hash do nodo
 * @param cntt Conteúdo novo
 * @return AVL com o nodo alterado
 */

void *addAVL(AVL tree, char *hash) {
	int update = 0;
	NODE new, last;

	new  = newNode(hash, NULL, NULL, NULL);

	tree->head = insertNode(tree->head, new, &update, &last);

	if (last == new){
		tree->size++;
		new->content = tree->init();
		return new;
	}else{
		free(new);
		return last->content;
	} 
}

/**
 * Substitui o conteúdo atual do elemento com a hash indicada com o novo conteúdo, libertando o conteúdo antigo
 * @param tree Árvore com o elemento a ser modificado
 * @param hash Identificador do elemento
 * @param content Novo conteúdo do elemento
 * @result Conteúdo antigo do nodo
 */
void *replaceAVL(AVL tree, char *hash, void *content) {
	NODE p;
	void *oldContent = NULL;
	int res;
	bool stop;

	p = tree->head;
	stop = false;

	while(p && !stop) {
		res = strcmp(hash, p->hash);

		if (res > 0)
			p = p->right;
		else if (res < 0)
			p = p->left;
		else {
			oldContent = p->content;

			p->content = content;
			stop = true;
		}
	}

	return oldContent;
}

AVL cloneAVL(AVL tree, void* (*init)(),
             bool (*equals)(void*, void*), void* (*clone)(void*), void (*free)  (void *)){

	AVL new = initAVL(init, equals, clone, free);
	new->size = tree->size;
	new->head = cloneNode(tree->head, tree->clone);

	return new;
}

NODE cloneNode(NODE n, void* (*clone)(void *)) {

	if (n) {
		NODE new;
	
		new = malloc(sizeof(*new));
		new->hash = malloc(sizeof(char) * HASH_SIZE);
	
		strncpy(new->hash, n->hash, HASH_SIZE);
		new->bal = n->bal;
		new->content = (clone) ? clone(n->content) : NULL;
		new->left = cloneNode(n->left, clone);
		new->right = cloneNode(n->right, clone);
	
		return new;
	}

	return NULL;
}

/**
 * Conta o número de elementos presentes na árvore
 * @param tree Árvore
 * @return Número de elementos
 */
int countNodes(AVL tree) {
	return tree->size;
}

/**
 * @param tree Árvore a ser procurada
 * @param hash Hash do element a encontrar
 * @return se existir retorna o conteúdo do elemento, senão NULL
 */
void *getAVLcontent(AVL tree, char *hash) {
	int res;
	NODE p = tree->head;
	
	while(p) {
		res = strcmp(hash, p->hash);

		if (res > 0)
			p = p->right;
		else if (res < 0)
			p = p->left;
		else
			return p->content;
	}
	
	return NULL;
}

/**
 * Verifica se uma dada AVL é vazia ou não.
 * @param tree Árvore a ser verificada
 * @return true caso seja vazia, false caso contrário.
 */
bool isEmptyAVL(AVL tree) {
	return (tree->size == 0);
}

/**
 * Verifica se duas árvores são iguais
 * @param a Árvore alvo da verificação
 * @param b Árvore alvo da verificação
 * @result true caso sejam iguais, false caso contrário
 */
bool equalsAVL(AVL a, AVL b) {
	if (a->equals == b->equals)
		return equalsNode(a->head, b->head, a->equals);

	return false;
}

bool equalsNode(NODE a, NODE b, bool (*equals)(void*, void*)) {
	bool sameHash, sameContent = true;

	if (!a && !b)
		return true;

	if (!a || !b)
		return false;

	sameHash = !strcmp(a->hash, b->hash);

	if (equals)
		sameContent = equals(a->content, b->content);

	if (sameHash && sameContent)
		return equalsNode(a->left,  b->left,  equals) && 
			   equalsNode(a->right, b->right, equals);

	return false;
}

/**
 * Dado um catálogo e uma string verifica se existe essa string na AVL.
 * @param tree AVL
 * @param hash String a procurar
 * @return true caso encontre, false caso contrário
 */
bool lookUpAVL(AVL tree, char *hash) {
	NODE p = tree->head;
	int res;

	while(p){
		res = strcmp(hash, p->hash);

		if (res > 0)
			p = p->right;
		else if (res < 0)
			p = p->left;
		else
			return true;
	}

	return false;
}

/**
 * Liberta o espaço ocupado por uma AVL
 * @param p AVL a libertar
 */
void freeAVL(AVL tree) {
	if (tree){
		freeNode(tree->head, tree->free);
		free(tree);
	}	
}

static void freeNode(NODE node, void (*freeContent)(void*)) {
	if (node) {
		freeNode(node->left, freeContent);
		freeNode(node->right, freeContent);
	
		free(node->hash);

		if (freeContent)
			freeContent(node->content);

		free(node);
	}
}


DATASET unionDataSets(DATASET dest, DATASET source) {
	DATASET new = initDataSet(100);
	int res, sourceSize, destSize, maxSourceSize, maxDestSize;
	
	sourceSize = destSize = 0;
	maxSourceSize = source->pos;
	maxDestSize = dest->pos;

	while(sourceSize < maxSourceSize && destSize < maxDestSize){
		res = strcmp(dest->set[destSize]->hash, source->set[sourceSize]->hash);

		if (res < 0) {
			new = insertDataSet(new, dest->set[destSize]);
			destSize++;
		}else if (res > 0){
			new = insertDataSet(new, source->set[sourceSize]);
			sourceSize++;
		}else{
			new = insertDataSet(new, dest->set[destSize]);
			sourceSize++;	
			destSize++;
		}
	}

	for(; destSize < maxDestSize; destSize++)
		new = insertDataSet(new, dest->set[destSize]);

	for(; sourceSize < maxSourceSize; sourceSize++)
		new = insertDataSet(new, source->set[sourceSize]);

	free(dest->set);
	
	dest->set = new->set;
	dest->pos = new->pos;
	dest->size = new->size;
	free(new);
	
	return dest;
}


DATASET diffDataSets(DATASET dest, DATASET source) {
	DATASET new = initDataSet(100);
	int res, destSize, sourceSize, maxDestSize, maxSourceSize;

	destSize = sourceSize = 0;
	maxDestSize = dest->pos;
	maxSourceSize = source->pos;

	while(destSize < maxDestSize && sourceSize < maxSourceSize){
		res = strcmp(dest->set[destSize]->hash, source->set[sourceSize]->hash);

		if (res < 0) {
			new = insertDataSet(new, dest->set[destSize]);
			destSize++;
		}else if (res > 0){
			new = insertDataSet(new, source->set[sourceSize]);
			sourceSize++;
		}else{
			destSize++;
			sourceSize++;
		}
	}

	for(; destSize < maxDestSize; destSize++)
		new = insertDataSet(new, dest->set[destSize]);

	for(; sourceSize < maxSourceSize; sourceSize++)
		new = insertDataSet(new, source->set[sourceSize]);
	
	free(dest->set);
	
	dest->set = new->set;
	dest->pos = new->pos;
	dest->size = new->size;
	free(new);

	return new;
}

static NODE newNode(char *hash, void *content, NODE left, NODE right) {
	NODE new = malloc(sizeof(struct node));

	new->bal = EH;
	new->hash = malloc(sizeof(char)*HASH_SIZE);
	new->content = content;
	new->left = left;
	new->right = right;
	strncpy(new->hash, hash, HASH_SIZE);

	return new;
}

/* Rotação à direita da árvore */
static NODE rotateRight(NODE node) {
	NODE aux = NULL;

	if (!node || !(node->left))
		return 0;

	aux = node->left;
	node->left = aux->right;
	aux->right = node;
	node = aux;

	return node;
}

/* Rotação à esquerda da árvore */
static NODE rotateLeft(NODE node) {
	NODE aux = NULL;

	if (!node || !(node->right))
		return 0;

	aux = node->right;
	node->right = aux->left;
	aux->left = node;
	node = aux;

	return node;
}

/* Balança a árvore caso esteja inclinada para a direita*/
static NODE balanceRight(NODE node) {

	if (node->right->bal == RH) {
		/* Se o nodo da direita está inclinado para a direita*/
		node = rotateLeft(node);
		node->bal = EH;
		node->left->bal = EH;
	} else {
		/* Se o nodo da direita está inclinado para a esquerda*/
		node->right = rotateRight(node->right);
		node = rotateLeft(node);

		switch (node->bal) {
			case EH:
				node->left->bal = EH;
				node->right->bal = EH;
				break;
			case LH:
				node->left->bal = EH;
				node->right->bal = RH;
				break;
			case RH:
				node->left->bal = LH;
				node->right->bal = EH;
		}
		node->bal = EH;
	}

	return node;
}

/* Balança a árvore caso esteja inclinada para a esquerda */
static NODE balanceLeft(NODE node) {

	if (node->left->bal == LH) {
		/* Se o nodo da esquerda está inclinado para a esquerda*/
		node = rotateRight(node);
		node->bal = EH;
		node->right->bal = EH;
	} else {
		/* Se o nodo da esquerda está inclinado para a direita*/
		node->left = rotateLeft(node->left);
		node = rotateRight(node);

		switch (node->bal) {
			case EH:
				node->left->bal = EH;
				node->right->bal = EH;
				break;
			case LH:
				node->left->bal = EH;
				node->right->bal = RH;
				break;
			case RH:
				node->left->bal = LH;
				node->right->bal = EH;
		}
		node->bal = EH;
	}

	return node;
}

/* Insere um novo Nodo à direita.*/
static NODE insertRight(NODE node, NODE new, int *update, NODE *last) {
	node->right = insertNode(node->right, new, update, last);

	if (*update) {
		switch (node->bal) {
			case LH:
				node->bal = EH;
				*update = 0;
				break;
			case EH:
				node->bal = RH;
				*update = 1;
				break;
			case RH:
				node = balanceRight(node);
				*update = 0;
				break;
		}
	}

	return node;
}

/* Insere um novo Nodo à esquerda. */
static NODE insertLeft(NODE node, NODE new, int *update, NODE *last) {
	node->left = insertNode(node->left, new, update, last);

	if (*update) {
		switch (node->bal) {
			case RH:
				node->bal = EH;
				*update = 0;
				break;
			case EH:
				node->bal = LH;
				*update = 1;
				break;
			case LH:
				node = balanceLeft(node);
				*update = 0;
				break;
		}
	}

	return node;
}

/* Insere o novo Nodo na AVL.*/
static NODE insertNode(NODE node, NODE new, int *update, NODE *last) {
	int res;

	if (!node) {
		*update = 1;
		*last = new;
		return new;
	}

	res = strcmp(new->hash, node->hash);
	if (res > 0)
		node = insertRight(node, new, update, last);
	else if (res < 0)
		node = insertLeft(node, new, update, last);

	*last = node;
	return node;
}

DATASET initDataSet(int n) {
	DATASET new = malloc(sizeof(*new));

	new->size = n;
	new->pos = 0;
	new->set = malloc(sizeof(NODE) * n);

	return new;
}

static DATASET insertDataSet(DATASET ds, NODE data) {
	
	if (ds->pos == ds->size) {
		ds->size *= 2;
		ds->set = realloc(ds->set, ds->size * sizeof(NODE));
	}
	
	ds->set[ds->pos] = data;
	ds->pos++;

	return ds;
}

DATASET addDataSet(DATASET ds, AVL tree) {
	ds = addDataSetAux(ds, tree->head);
	return ds;	
}

DATASET addDataSetAux(DATASET ds, NODE node) {

	if (node) {
		ds = addDataSetAux(ds, node->left);
		ds = insertDataSet(ds, node);
		ds = addDataSetAux(ds, node->right);
	}

	return ds;
}

void* getDataPos(DATASET ds, int pos) {
	if (pos < 0 || pos >= ds->pos)
		return NULL;

	return ds->set[pos]->content;
}

char* getHashPos(DATASET ds, int pos) {
	if (pos < 0 || pos >= ds->pos)
		return NULL;

	return ds->set[pos]->hash;
}

int getDataSetSize(DATASET ds) { 
	return ds->pos;
}

DATASET datacpy (DATASET dest, DATASET src, int i) {
	NODE n = src->set[i];
	insertDataSet(dest, n);

	return dest;
}

void freeDataSet(DATASET ds) {
	free(ds->set);
	free(ds);
}

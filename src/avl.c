#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "avl.h"

#define BUFFER_SIZE 10

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

	void* (*join)   (void *newContent, void *oldContent);
	bool  (*equals) (void *content1, void *content2);
	void* (*clone)  (void *content);
	void  (*free)   (void *content);
};

struct hashSet {
	int size;
	int sp;
	char **set;
};

static NODE newNode(char *hash, void *content, NODE left, NODE right);
static NODE rotateRight(NODE node);
static NODE rotateLeft(NODE node);
static NODE balanceRight(NODE node);
static NODE balanceLeft(NODE node);
static NODE insertRight(NODE node, NODE new, int *update);
static NODE insertLeft(NODE node, NODE new, int *update);
static NODE insertNode(NODE node, NODE new, int *update);
static bool equalsNode(NODE a, NODE b, bool (*equals)(void*, void*));
static void freeNode(NODE node, void (*freeContent)(void *));
static HASHSET fillHashSetaux(HASHSET hs, NODE n);
static HASHSET insertHashSet(HASHSET hs, char *hash); 

/**
 * Inicia uma nova AVL.
 * @return Nova AVL
 */
AVL initAVL(void* (*join)(void*, void*), bool (*equals)(void*, void*), 
            void* (*clone)(void*), void (*free)(void *)){

	AVL tree = malloc (sizeof (struct avl));

	tree->head= NULL;
	tree->size = 0;

	tree->join = join;
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
	NODE new = newNode(hash, content, NULL, NULL);

	tree->head = insertNode(tree->head, new, &update);
	tree->size++;

	return tree;
}

/**
 * Atualiza o conteúdo de um nodo da AVL caracterizado por uma Hash
 * @param tree AVL
 * @param hsh Hash do nodo
 * @param cntt Conteúdo novo
 * @return AVL com o nodo alterado
 */
AVL updateAVL(AVL tree, char *hash, void *content) {
	NODE p;
	int res;
	bool stop;

	p = tree->head;
	stop = false;

	while (p && !stop) {
		res = strcmp(hash, p->hash);

		if (res > 0)
			p = p->right;
		else if (res < 0)
			p = p->left;
		else {
			p->content = tree->join(content, p->content); 
			stop = 1;	
		}
	}

	return tree;
}

/**
 * Substitui o conteúdo atual do elemento com a hash indicada com o novo conteúdo, libertando o conteúdo antigo
 * @param tree Árvore com o elemento a ser modificado
 * @param hash Identificador do elemento
 * @param content Novo conteúdo do elemento
 * @result Árvore com o elemento substituido
 */
AVL replaceAVL(AVL tree, char *hash, void *content) {
	NODE p;
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
			tree->free(p->content);
			p->content = content;
			stop = true;
		}
	}

	return tree;
}

/**
 * Devolve um clone de uma dada AVL, copiando o seu conteúdo com uma dada função
 * @param p AVL a clonar
 * @param cloneCntt Função auxiliar para clonar o conteúdo.
 * @return Nova AVL
 */
AVL cloneAVL (AVL tree) { 
	QUEUE q = initQueue();
	AVL new = initAVL(tree->join, tree->equals, tree->clone, tree->free);
	NODE n;
	void* cntt = NULL;

	n = tree->head;
	new->size = tree->size;

	q = enqueue(q, n);	

	while((n = dequeue(q))) {

		if (tree->clone) 
			cntt = tree->clone(n->content);

		new = insertAVL(new, n->hash, cntt);
		
		if (n->left)  q = enqueue(q,n->left);
		if (n->right) q = enqueue(q,n->right);
	}

	freeQueue(q);
	return new; 
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
	if (!tree){
		freeNode(tree->head, tree->free);
		free(tree);
	}	
}

static void freeNode(NODE node, void (*freeContent)(void*)) {
	if (!node) {
		free(node->hash);
	
		if (freeContent)
			freeContent(node->hash);

		freeNode(node->left, freeContent);
		freeNode(node->right, freeContent);
		free(node);
	}
}

HASHSET initHashSet(int n) {

	HASHSET new = malloc(sizeof(*new));

	new->size = n;
	new->sp = 0;
	new->set = malloc(sizeof(char *) * n);

	return new;
}

static HASHSET insertHashSet(HASHSET hs, char *hash) {
	
	if (hs->sp == hs->size) {
		hs->size *= 2;
		hs->set = realloc(hs->set, hs->size * sizeof(char *));
	}
	
	hs->set[hs->sp] = hash;
	hs->sp++;

	return hs;
}


HASHSET fillHashSet(HASHSET hs, AVL tree) {
	
	if (tree) hs = fillHashSetaux(hs, tree->head);

	return hs;
}

char* getHashSetPos(HASHSET hs, int pos) {
	return hs->set[pos];
}

int getHashSetSize(HASHSET hs) {
	return hs->sp;
}

static HASHSET fillHashSetaux(HASHSET hs, NODE n) {

	if (n) {
		hs = fillHashSetaux(hs, n->left);
		hs = insertHashSet(hs, n->hash);
		hs = fillHashSetaux(hs, n->right);
	}

	return hs;
}

HASHSET unionHSets(HASHSET hs1, HASHSET hs2) {
	HASHSET new = initHashSet(100);
	int res, s1, s2, s1_max, s2_max;
	
	s1 = s2 = 0;
	s1_max = hs1->sp;
	s2_max = hs2->sp;

	while(s1 < s1_max && s2 < s2_max){
		res = strcmp(hs1->set[s1], hs2->set[s2]);

		if (res < 0) {
			new = insertHashSet(new, hs1->set[s1]);
			s1++;
		}else if (res > 0){
			new = insertHashSet(new, hs2->set[s2]);
			s2++;
		}else{
			new = insertHashSet(new, hs2->set[s2]);
			s1++;
			s2++;
		}
	}

	for(; s1 < s1_max; s1++)
		new = insertHashSet(new, hs1->set[s1]);

	for(; s2 < s2_max; s2++)
		new = insertHashSet(new, hs2->set[s2]);

	return new;
}

HASHSET diffHSets(HASHSET hs1, HASHSET hs2) {
	HASHSET new = initHashSet(100);
	int res, s1, s2, s1_max, s2_max;

	s1 = s2 = 0;
	s1_max = hs1->sp;
	s2_max = hs2->sp;

	while(s1 < s1_max && s2 < s2_max){
		res = strcmp(hs1->set[s1], hs2->set[s2]);

		if (res < 0) {
			new = insertHashSet(new, hs1->set[s1]);
			s1++;
		}else if (res > 0){
			new = insertHashSet(new, hs2->set[s2]);
			s2++;
		}else{
			s1++;
			s2++;
		}
	}

	for(; s1 < s1_max; s1++)
		new = insertHashSet(new, hs1->set[s1]);

	for(; s2 < s2_max; s2++)
		new = insertHashSet(new, hs2->set[s2]);

	return new;
}

void freeHashSet(HASHSET hs) {
	free(hs);
}

static NODE newNode(char *hash, void *content, NODE left, NODE right) {
	NODE new = malloc(sizeof(struct node));

	new->bal = EH;
	new->hash = malloc(sizeof(BUFFER_SIZE));
	new->content = content;
	new->left = left;
	new->right = right;
	strncpy(new->hash, hash, BUFFER_SIZE);

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
static NODE insertRight(NODE node, NODE new, int *update) {
	node->right = insertNode(node->right, new, update);

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
static NODE insertLeft(NODE node, NODE new, int *update) {
	node->left = insertNode(node->left, new, update);

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
static NODE insertNode(NODE node, NODE new, int *update) {
	int res;

	if (!node) {
		*update = 1;
		return new;
	}

	res = strcmp(new->hash, node->hash);
	if (res > 0)
		node = insertRight(node, new, update);
	else if (res < 0)
		node = insertLeft(node, new, update);

	return node;
}

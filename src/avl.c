#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"
#include "avl.h"

#define BUFFER_SIZE 10

typedef enum balance { LH, EH, RH } Balance;

struct avl {
	Balance bal;
	char *hash;
	void *content;
	struct avl *left;
	struct avl *right;
};

static AVL insertAVLaux(AVL p, AVL new, int *update);
static AVL newABin(char *hash, void *content, AVL left, AVL right);

/**
 * Inicia uma nova AVL.
 * @return Nova AVL
 */
AVL initAVL() {
	return NULL;
}

/**
 * Insere conteúdo na AVL com Hash característica do Nodo.
 * @param p AVL a onde inserir
 * @param s Hash a inserir
 * @param c Conteúdo a inserir
 * @return AVL com o novo nodo.
 */
AVL insertAVL(AVL p, char *s, void *c) {
	int update=0;
	AVL new = newABin(s, c, NULL, NULL);

	return insertAVLaux(p, new, &update);
}

/**
 * Atualiza o conteúdo de um nodo da AVL caracterizado por uma Hash
 * @param p AVL
 * @param hsh Hash do nodo
 * @param cntt Conteúdo novo
 * @return AVL com o nodo alterado
 */
AVL updateAVL(AVL p, char *hsh, void *cntt) {

	int r, stop=0;
	AVL a = p;

	while (a && !stop) {
			r = strcmp(hsh, p->hash);

		if (r > 0)
			a = a->right;
		else if (r < 0)
			a = a->left;
		else {
			a->content = cntt; 
			stop = 1;	
		}
	}

	return p;
}


/**
 * Devolve um clone de uma dada AVL, copiando o seu conteúdo com uma dada função
 * @param p AVL a clonar
 * @param cloneCntt Função auxiliar para clonar o conteúdo.
 * @return Nova AVL
 */
AVL cloneAVL (AVL p, void* (*cloneCntt) (void * cntt)) {

	QUEUE q = initQueue();	
	AVL new = initAVL();
	void* cnttAux = NULL;

		q = enqueue(q, p);	

	while (p) {
		if (p->left)  q = enqueue(q, p->left);
		if (p->right) q = enqueue(q, p->right);

		if (!isEmptyQueue(q)) {
			p = dequeue(q);
			if (cloneCntt) cnttAux = cloneCntt(cnttAux); 
			new = insertAVL(new, p->hash, cnttAux);
		}
	}

	return new;
}

/**
 * Verifica se uma dada AVL é vazia ou não.
 * @return true caso seja vazia, false caso contrário.
 */
bool isEmptyAVL(AVL n) {
	return (n == NULL);
}

/**
 * Dado um catálogo e uma string verifica se existe essa string na AVL.
 * @param n AVL
 * @param s String a procurar
 * @return true caso encontre, false caso contrário
 */
bool lookUpAVL(AVL n, char *s) {
	int r;

	while(n){
		r = strcmp(s, n->hash);

		if (r > 0)
			n = n->right;
		else if (r < 0)
			n = n->left;
		else
			return true;
	}

	return false;
}

/**
 * Liberta o espaço ocupado por uma AVL
 * @param p AVL a libertar
 */
void freeAVL(AVL p) {
	if (!p){
		free(p->hash);
		freeAVL(p->left);
		freeAVL(p->right);
		free(p);
	}
}

void printInOrderAVL(AVL p) {
	if (!p)
		putchar ('\n');
	else {
		printInOrderAVL(p->left);
		printf("|%s|\n", p->hash);
		printInOrderAVL(p->right);
	}
}

static AVL newABin(char *hsh, void *contt, AVL left, AVL right) {
	AVL new = malloc(sizeof(struct avl));

	new->bal = EH;
	new->hash = malloc(sizeof(BUFFER_SIZE));
	new->content = contt;
	new->left = left;
	new->right = right;
	strncpy(new->hash, hsh, BUFFER_SIZE);

	return new;
}

/* Rotação à direita da árvore */
static AVL rotateRight(AVL p) {
	AVL aux = NULL;

	if (!p || !(p->left))
		return 0;

	aux = p->left;
	p->left = aux->right;
	aux->right = p;
	p = aux;

	return aux;
}

/* Rotação à esquerda da árvore */
static AVL rotateLeft(AVL p) {
	AVL aux = NULL;

	if (!p || !(p->right))
		return 0;

	aux = p->right;
	p->right = aux->left;
	aux->left = p;
	p = aux;

	return aux;
}

/* Balança a árvore caso esteja inclinada para a direita*/
static AVL balanceRight(AVL p) {

	if (p->right->bal == RH) {
		/* Se o nodo da direita está inclinado para a direita*/
		p = rotateLeft(p);
		p->bal = EH;
		p->left->bal = EH;
	} else {
		/* Se o nodo da direita está inclinado para a esquerda*/
		p->right = rotateRight(p->right);
		p = rotateLeft(p);

		switch (p->bal) {
			case EH:
				p->left->bal = EH;
				p->right->bal = EH;
				break;
			case LH:
				p->left->bal = EH;
				p->right->bal = RH;
				break;
			case RH:
				p->left->bal = LH;
				p->right->bal = EH;
		}
		p->bal = EH;
	}

	return p;
}

/* Balança a árvore caso esteja inclinada para a esquerda */
static AVL balanceLeft(AVL p) {

	if (p->left->bal == LH) {
		/* Se o nodo da esquerda está inclinado para a esquerda*/
		p = rotateRight(p);
		p->bal = EH;
		p->right->bal = EH;
	} else {
		/* Se o nodo da esquerda está inclinado para a direita*/
		p->left = rotateLeft(p->left);
		p = rotateRight(p);

		switch (p->bal) {
			case EH:
				p->left->bal = EH;
				p->right->bal = EH;
				break;
			case LH:
				p->left->bal = EH;
				p->right->bal = RH;
				break;
			case RH:
				p->left->bal = LH;
				p->right->bal = EH;
		}
		p->bal = EH;
	}

	return p;
}

/* Insere um novo Nodo à direita.*/
static AVL insertRight(AVL p, AVL new, int *update) {
	p->right = insertAVLaux(p->right, new, update);

	if (*update) {
		switch (p->bal) {
			case LH:
				p->bal = EH;
				*update = 0;
				break;
			case EH:
				p->bal = RH;
				*update = 1;
				break;
			case RH:
				p = balanceRight(p);
				*update = 0;
				break;
		}
	}

	return p;
}

/* Insere um novo Nodo à esquerda. */
static AVL insertLeft(AVL p, AVL new, int *update) {
	p->left = insertAVLaux(p->left, new, update);

	if (*update) {
		switch (p->bal) {
			case RH:
				p->bal = EH;
				*update = 0;
				break;
			case EH:
				p->bal = LH;
				*update = 1;
				break;
			case LH:
				p = balanceLeft(p);
				*update = 0;
				break;
		}
	}

	return p;
}

/* Insere o novo Nodo na AVL.*/
static AVL insertAVLaux(AVL p, AVL new, int *update) {
	int r;

	if (!p) {
		*update = 1;
		return new;
	}

	r = strcmp(new->hash, p->hash);
	if (r > 0)
		p = insertRight(p, new, update);
	else if (r < 0)
		p = insertLeft(p, new, update);

	return p;
}

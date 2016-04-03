#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "avl.h"

#define BUFFER_SIZE 10

static AVL insertAVLaux(AVL p, AVL new, int *update);
static AVL newABin(char *buffer, AVL left, AVL right);

/**
 * Inicia um novo nodo.
 * @return Novo nodo
 */
AVL initAVL() {
	return NULL;
}

/**
 * Insere uma string na árvore
 * @param p Árvore onde inserir
 * @param s String a inserir
 * @return Árvore com o novo nodo.
 */
AVL insertAVL(AVL p, char *s) {
	int update=0;
	AVL new = newABin(s, NULL, NULL);

	return insertAVLaux(p, new, &update);
}

/**
 * Verifica se uma dada Árvore é vazia ou não.
 * @return true caso seja vasia, false caso contrário.
 */
bool isEmptyAVL(AVL n) {
	return (n == NULL);
}

/**
 * Dado um catálogo e uma string verifica se existe essa string na árvore.
 * @param n Árvore
 * @param s String a procurar
 * @return true caso encontre, false caso contrário
 */
bool lookUpAVL(AVL n, char *s) {
	int r;

	while(n){
		r = strcmp(s, n->str);

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
 * Liberta o espaço ocupado por uma árvore 
 * @param p Árvore a libertar
 * @return void
 */
void freeAVL(AVL p) {
	if (!p){
		free(p->str);
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
		printf("|%s|\n", p->str);
		printInOrderAVL(p->right);
	}
}

static AVL newABin(char *buffer, AVL left, AVL right) {
	AVL new = malloc(sizeof(struct avl));

	new->bal = EH;
	new->str = malloc(sizeof(BUFFER_SIZE));
	new->left = left;
	new->right = right;
	strncpy(new->str, buffer, BUFFER_SIZE);

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


/* Insere o novo Nodo na árvore.*/
static AVL insertAVLaux(AVL p, AVL new, int *update) {
	int r;

	if (!p) {
		*update = 1;
		return new;
	}

	r = strcmp(new->str, p->str);
	if (r > 0)
		p = insertRight(p, new, update);
	else if (r < 0)
		p = insertLeft(p, new, update);

	return p;
}

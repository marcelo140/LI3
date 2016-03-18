#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "avl.h"

#define BUFFER_SIZE 10
static NODE insertNode(NODE p, NODE new, int *update);
static void printNodeInOrder(NODE p);

CATALOG initCatalog () {
	CATALOG c = malloc(sizeof (*c));
	int i;

	for (i=0; i<26; i++) c->root[i] = NULL;

	return c;
}

static NODE newABin(char *buffer, NODE left, NODE right){
	NODE new = malloc(sizeof(struct node));

	new->bal = EH;
	new->str = malloc(sizeof(BUFFER_SIZE));
	new->left = left;
	new->right = right;
	strncpy(new->str, buffer, BUFFER_SIZE);

	return new;
}

int insert(CATALOG c, char *buffer) {

	int pos = buffer[0] - 'A', update=0;
	NODE p = c->root[pos];
	NODE new = newABin(buffer, NULL, NULL);

	c->root[pos] = insertNode(p, new, &update);

	return 0;
}

/*Dado um catálogo e uma String localiza essa string no catálogo.*/
int lookUp(CATALOG c, char *buffer) {
	int r, pos = buffer[0] - 'A';
	NODE p = c->root[pos];

	while(p != NULL){
		r = strcmp(buffer, p->str);

		if (r > 0)
			p = p->right;
		else if (r < 0)
			p = p->left;
		else
			return 1;
	}

	return 0;
}

static void printNodeInOrder(NODE p) {

	if (!p) putchar ('\n');
	else {
		printNodeInOrder(p->left);
		printf("|%s|\n", p->str);
		printNodeInOrder(p->right);
	}
}

void printInOrder (CATALOG c) {

	int i;

	if (!c) putchar('*');
	else {
		for (i=0; i<26; i++)
			printf("==== %c:\n", 'A' +i);
			printNodeInOrder(c->root[i]);
	}
}

/* Rotação à direita da árvore */
static NODE rotateRight(NODE p) {
	NODE aux = NULL;

	if ( !p || !(p->left))
		fprintf(stderr, "ERRO - Tentativa de rotação à direita em árvore nula.\n");
	else {
		aux = p->left;
		p->left = aux->right;
		aux->right = p;
		p = aux;
	}

	return aux;
}

/* Rotação à esquerda da árvore */
static NODE rotateLeft(NODE p) {
	NODE aux = NULL;

	if ( !p || !(p->right))
		fprintf(stderr, "ERRO - Tentativa de rotação à esquerda em árvore nula.\n");
	else {
		aux = p->right;
		p->right = aux->left;
		aux->left = p;
		p = aux;
	}

	return aux;
}

/* Balança a árvore caso esteja inclinada para a direita*/
static NODE balanceRight(NODE p) {

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
static NODE balanceLeft(NODE p) {

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
static NODE insertRight(NODE p, NODE new, int *update) {
	p->right = insertNode(p->right, new, update);

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
static NODE insertLeft(NODE p, NODE new, int *update) {
	p->left = insertNode(p->left, new, update);

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
static NODE insertNode(NODE p, NODE new, int *update) {

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


/* Liberta o espaço ocupado por uma árvore */
void freeABin(NODE p){
	if (!p){
	free(p->str);
	freeABin(p->left);
	freeABin(p->right);
	free(p);
	}
}

/* Liberta todo o espaço ocupado pelo catálogo */
void freeCatalog(CATALOG c){

	int i;

	if (!c){
		for (i=0; i<26; i++) freeABin(c->root[i]);
		free(c);
	}
}

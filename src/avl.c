#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
};

static NODE newNode(char *hash, void *content, NODE left, NODE right);
static NODE rotateRight(NODE node);
static NODE rotateLeft(NODE node);
static NODE balanceRight(NODE node);
static NODE balanceLeft(NODE node);
static NODE insertRight(NODE node, NODE new, int *update);
static NODE insertLeft(NODE node, NODE new, int *update);
static NODE insertNode(NODE node, NODE new, int *update);
static bool equalsNode(NODE a, NODE b);
static void freeNode(NODE node);

/**
 * Inicia uma nova AVL.
 * @return Nova AVL
 */
AVL initAVL() {
	AVL tree = malloc (sizeof (struct avl));

	tree->head= NULL;
	tree->size = 0;

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
	int res, stop=0;
	NODE p = tree->head;

	while (p && !stop) {
		res = strcmp(hash, p->hash);

		if (res > 0)
			p = p->right;
		else if (res < 0)
			p = p->left;
		else {
			p->content = content; 
			stop = 1;	
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

/*
AVL cloneAVL (AVL p, void* (*cloneCntt) (void *cntt)) {
	QUEUE q = initQueue();
	AVL new = initAVL();
	void* cnttAux = NULL;

	q = enqueue(q, p);	

	while((p = dequeue(q))) {

		if (cloneCntt) cnttAux = cloneCntt(p->content);
		new = insertAVL(new, p->hash, cnttAux);
		
		if (p->left)  q = enqueue(q,p->left);
		if (p->right) q = enqueue(q,p->right);
	}

	freeQueue(q);
	return new; 
}
*/

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

bool equalsAVL(AVL a, AVL b) {
	return equalsNode(a->head, b->head);
}

bool equalsNode(NODE a, NODE b) {
	if (!a && !b)
		return true;

	if (!a || !b)
		return false;

	if (!strcmp(a->hash, b->hash) && a->content == b->content)
		return equalsNode(a->left, b->left) && equalsNode(a->right, b->right);

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
		freeNode(tree->head);
		free(tree);
	}	
}

static void freeNode(NODE node) {
	if (!node) {
		free(node->hash);
		freeNode(node->left);
		freeNode(node->right);
	}
}

/*
void printInOrderAVL(AVL p) {
	if (!p)
		putchar ('\n');
	else {
		printInOrderAVL(p->left);
		printf("|%s|\n", p->hash);
		printInOrderAVL(p->right);
	}
}
*/

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

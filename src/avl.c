#include <stdlib.h>
#include <string.h>

#include "avl.h"

typedef enum balance { LH, EH, RH } Balance;

typedef struct node {
	char* hash;
	void* content;
	struct node *left, *right;
	Balance bal;
} *NODE;

struct avl {
	NODE head;
	int size;

	init_t init;
	condition_t equals;
	clone_t clone;
	free_t free;
};

static NODE newNode      (char* hash, void* content, NODE left, NODE right);
static NODE insertNode   (NODE node, char* hash, void* content, int* update, NODE* last);
static NODE insertRight  (NODE node, char* hash, void* content, int* update, NODE* last);
static NODE insertLeft   (NODE node, char* hash, void* content, int* update, NODE* last);
static NODE balanceRight (NODE node);
static NODE balanceLeft  (NODE node);
static NODE rotateRight  (NODE node);
static NODE rotateLeft   (NODE node);
static NODE cloneNode    (NODE n, clone_t clone);

static bool equalsNode   (NODE a, NODE b, condition_t equals);
static void freeNode     (NODE node, free_t free);

static SET addNodeToSet (SET s, NODE node, clone_t clone);
static SET filterNode   (NODE n, SET s, clone_t clone, condition_t predicate, void* arg);

static SET dumpNode (NODE n, SET set, void*(*dumper)(void*));


AVL initAVL(init_t init, condition_t equals, clone_t clone, free_t free){
	AVL tree = malloc (sizeof (*tree));

	tree->head = NULL;
	tree->size = 0;

	tree->init = init;
	tree->equals = equals;
	tree->clone = clone;
	tree->free = free;

	return tree;
}

AVL changeOps (AVL tree, init_t init, condition_t equals, clone_t clone, free_t free){
	tree->init = init;
	tree->equals = equals;
	tree->clone = clone;
	tree->free = free;

	return tree;
}

AVL insertAVL(AVL tree, char *hash, void *content) {
	NODE last;
	int update = 0;

	tree->head = insertNode(tree->head, hash, content, &update, &last);

	if (update != -1) 
		tree->size++;

	return tree;
}

AVL cloneAVL(AVL tree) {
	AVL new = initAVL(tree->init, tree->equals, tree->clone, tree->free);

	new->size = tree->size;
	new->head = cloneNode(tree->head, tree->clone);

	return new;
}

void* replaceAVL(AVL tree, char* hash, void* content) {
	void* oldContent = NULL;
	NODE p = tree->head;
	int res;
	bool stop = false;

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

void *getAVLcontent(AVL tree, char *hash) {
	NODE p = tree->head;
	int res;
	
	while(p) {
		res = strcmp(hash, p->hash);

		if (res > 0)
			p = p->right;
		else if (res < 0)
			p = p->left;
		else{
			if (tree->init && !p->content)
				p->content = tree->init();
			return p->content;
		}
	}
	
	return NULL;
}

void* addAVL(AVL tree, char* hash) {
	NODE last;
	int update = 0;

	tree->head = insertNode(tree->head, hash, NULL, &update, &last);

	if (update != -1)
		tree->size++;

	if (!last->content)
		last->content = tree->init();

	return last->content;
}

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

bool equalsAVL(AVL a, AVL b) {
	if (a->equals == b->equals)
		return equalsNode(a->head, b->head, a->equals);

	return false;
}

bool isEmptyAVL(AVL tree) {
	return (tree->size == 0);
}

int countNodes(AVL tree) {
	return tree->size;
}

void freeAVL(AVL tree) {
	if (tree){
		freeNode(tree->head, tree->free);
		free(tree);
	}	
}

SET addAVLtoSet(SET s, AVL tree) {
	s = addNodeToSet(s, tree->head, tree->clone);

	return s;
}

SET filterAVL (AVL tree, SET s, condition_t condition, void* arg) {
    s = filterNode(tree->head, s, tree->clone, condition, arg);

	return s;
}

SET dumpAVL (AVL tree, SET set, void* (*dumper)(void*)){
	set = dumpNode(tree->head, set, dumper);

	return set;
}

static NODE newNode(char *hash, void *content, NODE left, NODE right) {
	NODE new = malloc(sizeof(struct node));
	int size = strlen(hash);

	new->bal = EH;
	new->hash = malloc(sizeof(char) * size + 1);
	new->content = content;
	new->left = left;
	new->right = right;
	strcpy(new->hash, hash);

	return new;
}

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

static NODE balanceRight(NODE node) {

	if (node->right->bal == RH) {
		node = rotateLeft(node);
		node->bal = EH;
		node->left->bal = EH;
	} else {
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

static NODE balanceLeft(NODE node) {

	if (node->left->bal == LH) {
		node = rotateRight(node);
		node->bal = EH;
		node->right->bal = EH;
	} else {
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

static NODE insertRight(NODE node, char* hash, void* content, int *update, NODE *last) {
	node->right = insertNode(node->right, hash, content, update, last);

	if (*update == 1) {
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

static NODE insertLeft(NODE node, char* hash, void* content, int *update, NODE *last) {
	node->left = insertNode(node->left, hash, content, update, last);

	if (*update == 1) {
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

static NODE insertNode(NODE node, char* hash, void* content, int *update, NODE *last) {
	NODE new;
	int res;

	if (!node) {
		*update = 1;
		new = newNode(hash, content, NULL, NULL);
		*last = new;
		return new;
	}

	res = strcmp(hash, node->hash);
	if (res > 0)
		node = insertRight(node, hash, content, update, last);
	else if (res < 0)
		node = insertLeft(node, hash, content, update, last);
	else {
		*update = -1;
		*last = node;
		return node;
	}

	return node;
}

static NODE cloneNode(NODE n, void* (*clone)(void *)) {

	if (n) {
		NODE new;
	
		new = malloc(sizeof(*new));
		new->hash = malloc(sizeof(char) * strlen(n->hash) + 1);
	
		strcpy(new->hash, n->hash);
		new->bal = n->bal;
		new->content = (clone) ? clone(n->content) : NULL;
		new->left = cloneNode(n->left, clone);
		new->right = cloneNode(n->right, clone);
	
		return new;
	}

	return NULL;
}

static bool equalsNode(NODE a, NODE b, bool (*equals)(void*, void*)) {
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


static SET filterNode(NODE n, SET s, clone_t clone, condition_t condition, void* arg) {
	void* contCopy = NULL;

	if (n) {
		s = filterNode(n->left, s, clone, condition, arg);
		
		if (condition(n->content, arg)){
			if (clone && n->content)
				contCopy = clone(n->content);

			s = insertElement(s, n->hash, contCopy);
		}

		s = filterNode(n->right, s, clone, condition, arg);
	}

	return s;
}

static SET dumpNode(NODE n, SET set, void* (*dumper)(void*)) {

	if (n) {
		set = dumpNode(n->left, set, dumper);
		set = insertElement(set, n->hash, dumper(n->content));
		set = dumpNode(n->right, set, dumper);	
	}

	return set;
}

static SET addNodeToSet(SET s, NODE node, clone_t clone) {
	void *contCopy = NULL;

	if (node) {
		s = addNodeToSet(s, node->left, clone);

		if (clone && node->content)
			contCopy = clone(node->content);

		insertElement(s, node->hash, contCopy);

		s = addNodeToSet(s, node->right, clone);
	}

	return s;
}

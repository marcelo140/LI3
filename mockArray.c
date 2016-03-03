#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "mockArray.h"

#define BUFFER_SIZE 10

CATALOG initCatalog () {
	CATALOG c = malloc(sizeof (*c));

	c->root = NULL;

	return c;
}

static NODE newABin(char *buffer, NODE left, NODE right){
	NODE new = malloc(sizeof(struct node));

	new->str = malloc(sizeof(BUFFER_SIZE));
	new->left = left;
	new->right = right;
	strncpy(new->str, buffer, BUFFER_SIZE);

	return new;
}

int insert(CATALOG c, char *buffer) {
	NODE p = c->root;
	NODE new = newABin(buffer, NULL, NULL);
	int r;

	while(p != NULL){
		r = strcmp(buffer, p->str);
		if (r != 0 && r > 0) {
			if (!p->right)
				{p->right = new; break; }
			else
				p = p->right;
		}else
			if (!p->left)
				{p->left = new; break; }
			else
				p = p->left;
	}

	if (!p)
		c->root = new;

	return 0;
}

int lookUp(CATALOG c, char *buffer) {
	NODE p = c->root;
	int r;

	while(p != NULL){
		r = strcmp(buffer, p->str);

		if (r > 0)
			p = p->right;
		else if (r < 0)
			p = p->left;
		else 
			return 0;
	}

	return -1;
}

void freeABin(NODE p){
	if (!p){
	free(p->str);
	freeABin(p->left);
	freeABin(p->right);
	free(p);
	}
}

void freeCatalog(CATALOG c){
	if (!c){
		freeABin(c->root);
		free(c);
	}
}

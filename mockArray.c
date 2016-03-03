#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "mockArray.h"

#define BUFFER_SIZE 10

CATALOG initCatalog (int tam) {
	CATALOG c = malloc(sizeof (*c));

	c->arr = malloc(sizeof(char *) * tam);
	c->size = 0;
	c->max = tam;

	return c;
}

int insert(CATALOG c, char *buffer) {
	if (c->size >= c->max)
		return 1;

	c->arr[c->size] = malloc(sizeof (char) * BUFFER_SIZE);
	strncpy(c->arr[c->size], buffer, BUFFER_SIZE);
	c->size++;

	return 0;
}

int lookUp(CATALOG c, char *buffer) {
	int i, s;
	s = c->size;

	for(i = 0; i < s; i++)
		if (!strcmp(c->arr[i], buffer))
			return i;

	return -1;
}

void freeCatalog(CATALOG c){
	int i, s;

	s = c->size;

	for (i = 0; i < s; i++)
		free(c->arr[i]);

	free(c);
}

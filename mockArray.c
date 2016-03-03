#include <stdlib.h>

#include "mockarray.h"

#define BUFFER_SIZE 10

catalog initArray (int tam) {
	catalog c = malloc(sizeof (char*) *  tam);
	
	c->arr = NULL;
	c->size = 0;
	c->max = tam;

	return c;
}

int insert(catalog c, char *buffer) {
	int s = catalog->size;

	if (s > catalog->max){
		return 1;

	c->arr[s] = malloc(sizeof (char) * BUFFER_SIZE);	
	strncpy(c->arr[s], buffer, BUFFER_SIZE);
	catalog->size++;

	return 0;
}

int lookUp(catalog c, char *buffer) {
	int i, s;
	s = c->size

	for(i = 0; i < s; i++)
		if (!strcmp(c->arr[i], buffer))
			return i;

	return -1;
}

void freeCatalog(catalog c){
	int i, s;
	
	s = c->size;

	for (i = 0; i < s; i++)
		free(c->arr[i]);

	free(c);
}

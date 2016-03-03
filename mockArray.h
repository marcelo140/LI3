#ifndef __MOCK_ARRAY_H__
#define __MOCK_ARRAY_H__

typedef struct mockArray {
	char **arr;
	int size;
	int max;
} catalog;

catalog initArray(int tam);
int insert(catalog c, char *buffer);
int lookUp(catalog c, char *buffer);
void freeCatalog(catalog c);

#endif

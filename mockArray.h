#ifndef __MOCK_ARRAY_H__
#define __MOCK_ARRAY_H__

typedef struct mockArray {
	char **arr;
	int size;
	int max;
} *CATALOG;

CATALOG initCatalog(int tam);
int insert(CATALOG c, char *buffer);
int lookUp(CATALOG c, char *buffer);
void freeCatalog(CATALOG c);

#endif

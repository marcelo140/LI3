#ifndef __MOCK_ARRAY_H__
#define __MOCK_ARRAY_H__

typedef struct node {
	char *str;
	struct node *left;
	struct node *right;
} *NODE;

typedef struct mockArray {
	NODE root[26];
} *CATALOG;

CATALOG initCatalog();
int insert(CATALOG c, char *buffer);
int lookUp(CATALOG c, char *buffer);
void freeCatalog(CATALOG c);

#endif

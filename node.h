#ifndef __AVL_H__
#define __AVL_H__

typedef enum balance { LH, EH, RH } Balance;

typedef struct node {
	Balance bal;
	char *str;
	struct node *left;
	struct node *right;
} *NODE;

NODE initNode();
NODE insertNode(NODE n, char *buffer);
int isEmptyNode(NODE n);
int lookUpNode(NODE n, char *buffer);
void freeNode(NODE n);
void printInOrderNode(NODE n);

#endif

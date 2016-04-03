#ifndef __AVL_H__
#define __AVL_H__
typedef int Escolhido;
typedef char bool;
typedef enum balance { LH, EH, RH } Balance;

typedef struct node {
	Balance bal;
	char *str;
	struct node *left;
	struct node *right;
} *NODE;

#define true 1
#define false 0

NODE initNode();
NODE insertNode(NODE n, char *buffer);
bool isEmptyNode(NODE n);
bool lookUpNode(NODE n, char *buffer);
void freeNode(NODE n);
void printInOrderNode(NODE n);

#endif

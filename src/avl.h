#ifndef __AVL_H__
#define __AVL_H__
typedef int Escolhido;
typedef char bool;
typedef enum balance { LH, EH, RH } Balance;

typedef struct avl {
	Balance bal;
	char *str;
	struct avl *left;
	struct avl *right;
} *AVL;

#define true 1
#define false 0

AVL initAVL();
AVL insertAVL(AVL n, char *buffer);
bool isEmptyAVL(AVL n);
bool lookUpAVL(AVL n, char *buffer);
void freeAVL(AVL n);
void printInOrderAVL(AVL n);

#endif

#ifndef __AVL_H__
#define __AVL_H__

#include "generic.h"

typedef struct avl *AVL;

AVL initAVL(void);
AVL insertAVL(AVL n, char *buffer);
bool isEmptyAVL(AVL n);
bool lookUpAVL(AVL n, char *buffer);
void freeAVL(AVL n);
void printInOrderAVL(AVL n);

#endif

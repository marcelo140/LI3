#ifndef __AVL_H__
#define __AVL_H__

#include "generic.h"

typedef struct avl *AVL;

AVL initAVL(void);
AVL insertAVL(AVL n, char *hash, void *content);
AVL updateAVL(AVL n, char *hash, void *content);
AVL cloneAVL (AVL p, void* (*cloneCntt) (void * cntt)); 
bool isEmptyAVL(AVL n);
bool lookUpAVL(AVL n, char *hash);
void *getAVLcontent(AVL tree, char *hash);
void freeAVL(AVL n);
void printInOrderAVL(AVL n);

#endif

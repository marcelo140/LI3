#ifndef __AVL_H__
#define __AVL_H__

#include "generic.h"

typedef struct avl *AVL;

AVL initAVL();
AVL insertAVL(AVL n, char *hash, void *content);
AVL updateAVL(AVL n, char *hash, void *content);
bool isEmptyAVL(AVL n);
int countNodes(AVL tree); 
bool lookUpAVL(AVL n, char *hash);
void *getAVLcontent(AVL tree, char *hash);
void freeAVL(AVL n);

AVL cloneAVL (AVL p, void* (*cloneCntt) (void * cntt));
void printInOrderAVL(AVL n);

#endif

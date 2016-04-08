#ifndef __AVL_H__
#define __AVL_H__

#include "generic.h"

typedef struct avl *AVL;
typedef struct hashSet *HASHSET;

AVL initAVL();
AVL insertAVL(AVL tree, char *hash, void *content);
AVL updateAVL(AVL n, char *hash, void *content);
bool isEmptyAVL(AVL n);
bool equalsAVL(AVL a, AVL b);
int countNodes(AVL tree); 
bool lookUpAVL(AVL tree, char *hash);
void *getAVLcontent(AVL tree, char *hash);
void freeAVL(AVL n);

HASHSET initHashSet(int n);
HASHSET insertHashSet(HASHSET hs, char *hash);
HASHSET getInOrderAVL(HASHSET hs, AVL tree);
char* getHashSetPos(HASHSET hs, int pos);
int getHashSetSize(HASHSET hs);
void freeHashSet(HASHSET set);

/*
AVL cloneAVL (AVL p, void* (*cloneCntt) (void * cntt));
void printInOrderAVL(AVL n);
*/

#endif

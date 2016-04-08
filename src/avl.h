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
HASHSET getInOrderAVL(HASHSET hs, AVL tree);
HASHSET unionHSets(HASHSET hs1, HASHSET hs2);
HASHSET diffHSets(HASHSET hs1, HASHSET hs2);
char* getHashSetPos(HASHSET hs, int pos);
int getHashSetSize(HASHSET hs);
void freeHashSet(HASHSET set);

AVL cloneAVL (AVL p, void* (*cloneCntt) (void * cntt));
/*
void printInOrderAVL(AVL n);
*/

#endif

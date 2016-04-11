#ifndef __AVL_H__
#define __AVL_H__

#include "generic.h"

typedef struct avl     *AVL;
typedef struct dataSet *DATASET;

AVL initAVL  (void* (*init)   (),
              void* (*join)   (void*, void*), 
              bool  (*equals) (void*, void*), 
              void* (*clone)  (void*), 
              void  (*free)   (void *));

AVL cloneAVL  (AVL tree,
               void* (*init)   (),
               void* (*join)   (void*, void*),
               bool  (*equals) (void*, void*), 
               void* (*clone)  (void*), 
               void  (*free)   (void *));

AVL insertAVL  (AVL tree, char *hash, void *content);
AVL updateAVL  (AVL tree, char *hash, void *content);

bool lookUpAVL  (AVL tree, char *hash);
bool equalsAVL  (AVL tree, AVL b);
bool isEmptyAVL (AVL tree);

void* replaceAVL    (AVL tree, char *hash, void *content);
void* getAVLcontent (AVL tree, char *hash);
int   countNodes    (AVL tree);
void  freeAVL       (AVL n);

/*
DATASET unionHSets    (DATASET hs1, DATASET hs2);
DATASET diffHSets     (DATASET hs1, DATASET hs2);
*/

DATASET initDataSet   (int n);
DATASET fillDataSet   (DATASET ds, AVL tree);
DATASET addDataSet    (DATASET ds, AVL tree);
DATASET joinDataSet   (DATASET dest, DATASET source);
DATASET datacpy       (DATASET dest, DATASET source, int pos);
DATASET clearDataSet  (DATASET ds);
void*   getDataPos    (DATASET ds, int pos);
char*   getHashPos    (DATASET ds, int pos);

int   getDataSetSize (DATASET ds);
void  freeDataSet    (DATASET set);

#endif

#ifndef __AVL__
#define __AVL__

#include "generic.h"

typedef struct avl      *AVL;
typedef struct data_set *DATASET;

AVL initAVL          (void* (*init)   (),
                      bool  (*equals) (void*, void*), 
                      void* (*clone)  (void*), 
                      void  (*free)   (void *));

AVL changeOperations (AVL tree,
                      void* (*init)   (),
                      bool  (*equals) (void*, void*),
                      void* (*clone)  (void*),
                      void  (*free)   (void*));

AVL    insertAVL     (AVL tree, char *hash, void *content);
AVL    cloneAVL      (AVL tree);
void*  replaceAVL    (AVL tree, char *hash, void *content);
void*  getAVLcontent (AVL tree, char *hash);

void*  addAVL        (AVL tree, char *hash);

bool lookUpAVL  (AVL tree, char *hash);
bool equalsAVL  (AVL tree, AVL b);
bool isEmptyAVL (AVL tree);
int  countNodes (AVL tree);

void  freeAVL       (AVL n);


DATASET initDataSet   (int n);
DATASET addDataSet    (DATASET ds, AVL tree);
DATASET datacpy       (DATASET dest, DATASET source, int pos);

DATASET unionDataSets (DATASET dest, DATASET src);
DATASET diffDataSets  (DATASET dest, DATASET src);

void*   getDataPos     (DATASET ds, int pos);
char*   getHashPos     (DATASET ds, int pos);
int     getDataSetSize (DATASET ds);

void  freeDataSet    (DATASET set);

#endif

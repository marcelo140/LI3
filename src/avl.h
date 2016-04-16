#ifndef __AVL__
#define __AVL__

#include "generic.h"

typedef struct avl      *AVL;
typedef struct data_set *DATASET;

AVL initAVL          (init_t init, condition_t equals, clone_t clone, free_t free);

AVL changeOperations (AVL tree, init_t init, condition_t equals,
                                clone_t clone, free_t free);

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

DATASET filterAVL     (AVL tree, DATASET ds, condition_t condition, void* arg);
void    separateAVL   (AVL tree, compare_t compare, void* arg, DATASET set1, DATASET set2);

void    condSeparateAVL (AVL tree, DATASET set1, DATASET set2,
                                   condition_t condition,  void* cond_arg,
                                   compare_t   comparator, void* comp_arg);
                
DATASET sortDataSet   (DATASET set1, compare_t comparator);                          
DATASET concatDataSet (DATASET set1, DATASET set2);
DATASET unionDataSets (DATASET dest, DATASET src);
DATASET diffDataSets  (DATASET dest, DATASET src);

void*   getDataPos     (DATASET ds, int pos);
char*   getHashPos     (DATASET ds, int pos);
int     getDataSetSize (DATASET ds);

void  freeDataSet    (DATASET set);

#endif

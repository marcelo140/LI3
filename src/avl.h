#ifndef __AVL_H__
#define __AVL_H__

#include "generic.h"

typedef struct avl *AVL;
typedef struct hashSet *HASHSET;

AVL initAVL  (void* (*join)   (void*, void*), 
              bool  (*equals) (void*, void*), 
              void* (*clone)  (void*), 
              void  (*free)   (void *));

AVL cloneAVL  (AVL tree,
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

HASHSET initHashSet   (int n);
HASHSET fillHashSet   (HASHSET hs, AVL tree);
HASHSET unionHSets    (HASHSET hs1, HASHSET hs2);
HASHSET diffHSets     (HASHSET hs1, HASHSET hs2);

char *getHashSetPos  (HASHSET hs, int pos);
int   getHashSetSize (HASHSET hs);
void  freeHashSet    (HASHSET set);

#endif

AVL insertAVL  (AVL tree, char *hash, void *content);
AVL updateAVL  (AVL tree, char *hash, void *content);
AVL cloneAVL   (AVL tree);

bool lookUpAVL  (AVL tree, char *hash);
bool equalsAVL  (AVL tree, AVL b);
bool isEmptyAVL (AVL tree);

void* replaceAVL    (AVL tree, char *hash, void *content);
void* getAVLcontent (AVL tree, char *hash);
int   countNodes    (AVL tree);
void  freeAVL       (AVL n);

HASHSET initHashSet   (int n);
HASHSET fillHashSet   (HASHSET hs, AVL tree);
HASHSET unionHSets    (HASHSET hs1, HASHSET hs2);
HASHSET diffHSets     (HASHSET hs1, HASHSET hs2);

char *getHashSetPos  (HASHSET hs, int pos);
int   getHashSetSize (HASHSET hs);
void  freeHashSet    (HASHSET set);

#endif

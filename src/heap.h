#ifndef __HEAP_H__
#define __HEAP_H__

typedef struct heap *HEAP;

HEAP initHeap();
HEAP insertHeap(HEAP h, void *key, void *content);
HEAP updateHeap(HEAP h, void *key, void *content);
int getHeapSize(HEAP h);
void *removeHeap(HEAP h, void **content);
void freeHeap();

#endif
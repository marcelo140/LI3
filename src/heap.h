#ifndef __HEAP_H__
#define __HEAP_H__

typedef struct heap *HEAP;

HEAP initHeap( 	void* (*init)(), 
				int (*compare)(void *key1, void *key2), 
			   	void* (*add) (void *oldContent, void *newContent)); 
/* A Heap precisa de mais dois frees */

HEAP  insertHeap  (HEAP h, void *key, void *content);
int   getHeapSize (HEAP h);
void* removeHeap  (HEAP h, void **content);

void freeHeap();

#endif

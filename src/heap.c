#include <stdlib.h>

#include "heap.h"

#define BASE_SIZE 100

#define RIGHT(i) (2*i)+2 
#define LEFT(i) (2*i)+1
#define PARENT(i) (i-1)/2

static void bubbleUp(HEAP h, int pos);
static void bubbleDown(HEAP h, int pos);
static void swap(HEAP h, int pos1, int pos2);

typedef struct heapNode {
	void *key;
	void *content;
}*HEAPNODE;

struct heap {
	struct heapNode **list;
	int size;
	int capacity;
	int (*compare)(void *key1, void *key2);
};

HEAP initHeap(int (*compare)(void *key1, void *key2)) {
	HEAP new = malloc(sizeof(*new));
	int i;

	new->list = malloc(sizeof(HEAPNODE) * BASE_SIZE);
	for(i = 0; i < BASE_SIZE; i++)
		new->list[i] = malloc(sizeof(struct heapNode));

	new->size = 0;
	new->capacity = BASE_SIZE;
	new->compare = compare;

	return new;
}

HEAP insertHeap(HEAP h, void *key, void *content) {
	HEAPNODE node = h->list[h->size];

	node->key = key;
	node->content = content;

	bubbleUp(h, h->size);
	h->size++;

	return h;	
}

void *removeHeap(HEAP h, void **content) {
	HEAPNODE node = h->list[0];

	h->size--;
	h->list[0] = h->list[h->size];
	bubbleDown(h, 0);

	if (content)
			*content = node->content;

	return node->key;
}

static void bubbleUp(HEAP h, int pos) {
	int (*comparator)(void *key1, void *key2) = h->compare;
	HEAPNODE *list = h->list;

	while(pos && comparator(list[pos]->key, list[PARENT(pos)]->key) < 0) {
		swap(h, pos, PARENT(pos));
		pos = PARENT(pos);
	}	
}

static void bubbleDown(HEAP h, int pos) {
	void *key1, *key2;
	HEAPNODE *list = h->list;
	int (*comparator)(void *key1, void *key2) = h->compare;
	int child;

	while(LEFT(pos) < h->size) {
		if (RIGHT(pos) >= h->size){
			child = LEFT(pos);
		}else{
			key1 = list[LEFT(pos)]->key;
			key2 = list[RIGHT(pos)]->key;
			child = (comparator(key1, key2) < 0) ? LEFT(pos) : RIGHT(pos);
		}

		if (comparator(list[child]->key, list[pos]->key) >= 0){
			break;
		}else{
			swap(h, pos, child);
			pos = child;
		}
	}
}

int getHeapSize(HEAP h) {
	return h->size;
}

static void swap(HEAP h, int pos1, int pos2) {
	HEAPNODE tmp = h->list[pos1];
	
	h->list[pos1] = h->list[pos2];
	h->list[pos2] = tmp;
}

void freeHeap(HEAP h) {
	if (!h)
		free(h->list);

	free(h);
}

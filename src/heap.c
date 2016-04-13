#include <stdlib.h>

#include "heap.h"

#define BASE_SIZE 100

#define RIGHT(i) (2*i)+2
#define LEFT(i) (2*i)+1
#define PARENT(i) (i-1)/2

#define COMPARE h->compare
#define LIST h->list
#define CONTENT(i) h->list[i]->content

static void bubbleUp(HEAP h, int pos);
static void bubbleDown(HEAP h, int pos);
static void swap(HEAP h, int pos1, int pos2);
static HEAP updateHeap(HEAP h, int pos, void *content);

typedef struct heapNode {
	void *key;
	void *content;
}*HEAPNODE;

struct heap {
	struct heapNode **list;
	int size;
	int capacity;
	void* (*init) ();
	int (*compare)(void *key1, void *key2);
	void * (*add) (void *oldContent, void *newContent);
};

HEAP initHeap(	void* (*init) (),
				int (*compare)(void *key1, void *key2), 
			 	void* (*add) (void *oldContent, void *newContent)) {
	HEAP new = malloc(sizeof(*new));
	int i;

	new->list = malloc(sizeof(HEAPNODE) * BASE_SIZE);
	for(i = 0; i < BASE_SIZE; i++)
		new->list[i] = malloc(sizeof(struct heapNode));

	new->size = 0;
	new->capacity = BASE_SIZE;
	new->init = init;
	new->compare = compare;
	new->add = add;

	return new;
}

HEAP insertHeap(HEAP h, void *key, void *content) {
	HEAPNODE node = LIST[h->size];
	int i, exist = 0;

	for (i=0; !exist && i < h->size; i++)
		exist = !COMPARE(LIST[i]->key, key);

	if (exist) h = updateHeap(h, i, content);
	else {
		node->key = key;
		node->content = h->init();
		h->add(node->content, content);

		bubbleUp(h, h->size);
		h->size++;
	}

	return h;
}

void *removeHeap(HEAP h, void **content) {
	HEAPNODE node = LIST[0];

	h->size--;
	h->list[0] = h->list[h->size];
	bubbleDown(h, 0);

	if (content)
		*content = node->content;

	return node->key;
}

static HEAP updateHeap(HEAP h, int pos, void *content) {

		CONTENT(pos) = h->add(CONTENT(pos), content);
		bubbleUp(h, pos);

		return h;
}

static void bubbleUp(HEAP h, int pos) {

	while(pos && COMPARE(LIST[pos]->key, LIST[PARENT(pos)]->key) < 0) {
		swap(h, pos, PARENT(pos));
		pos = PARENT(pos);
	}
}

static void bubbleDown(HEAP h, int pos) {
	void *key1, *key2;
	int child;

	while(LEFT(pos) < h->size) {
		if (RIGHT(pos) >= h->size){
			child = LEFT(pos);
		}else{
			key1 = LIST[LEFT(pos)]->key;
			key2 = LIST[RIGHT(pos)]->key;
			child = (COMPARE(key1, key2) < 0) ? LEFT(pos) : RIGHT(pos);
		}

		if (COMPARE(LIST[child]->key, LIST[pos]->key) >= 0){
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
	HEAPNODE tmp;

	tmp = LIST[pos1];
	LIST[pos1] = LIST[pos2];
	LIST[pos2] = tmp;
}

void freeHeap(HEAP h) {
	if (!h)
		free(h->list);

	free(h);
}

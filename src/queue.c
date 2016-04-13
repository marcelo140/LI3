#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue {
	void **queue;
	int capacity;
	int start;
	int pos;
	int size;
};

QUEUE initQueue(int n) {
	QUEUE q = malloc (sizeof(struct queue));
	q->queue = malloc (sizeof(void *) * n);
	q->capacity = n;
	q->start = 0;
	q->pos = 0;
	q->size = 0;

	return q;
}

QUEUE enqueue(QUEUE q, void *element) {

	if (q->size == q->capacity){
		q->queue = realloc(q->queue, 2*q->capacity*sizeof(void *));
		memcpy(&(q->queue[q->capacity]), q->queue, q->start * sizeof(void *));
		q->capacity *= 2;
	}
	
	q->queue[q->pos % q->capacity] = element;
	q->pos++;
	q->size++;

	return q;
}

bool isEmptyQueue(QUEUE q) {
	return (!q) ? true : (q->size == 0);
}

void *dequeue(QUEUE q) {
	void *element;
	
	if (q->size == 0)
		return NULL;

	element = q->queue[q->start];
	q->start++;
	q->size--;

	if (q->start == q->capacity)
		q->start = 0;

	return element;
}

void freeQueue(QUEUE q){
	free(q->queue);
	free(q);
}

#include <stdlib.h>

#include "queue.h"

struct queue {
	void *queue[BASE_SIZE];
	int start;
	int size;
	int capacity;
};

QUEUE initQueue() {
	QUEUE q = malloc (sizeof(*q));
	q->size = 0;
	q->start = 0;
	q->capacity = BASE_SIZE;

	return q;
}

QUEUE push(QUEUE q, void *element) {
	int pos;

	if (q->start + q->size == q->capacity){
		q->capacity = 2*q->capacity;
		q = realloc(q, q->capacity);
	}
	
	pos = (q->start + q->size) % q->capacity;
	q->queue[pos] = element;
	q->size++;

	if (q->size == q->capacity)
			q->size = 0;

	return q;
}

void *depush(QUEUE q) {
	void *element;

	element = q->queue[q->start];
	q->start++;

	if (q->start == q->capacity)
			q->start = 0;

	return element;
}

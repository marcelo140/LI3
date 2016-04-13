#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "generic.h"

typedef struct queue *QUEUE;

QUEUE initQueue(int n);
QUEUE enqueue(QUEUE q, void *element);
bool isEmptyQueue(QUEUE q);
void *dequeue(QUEUE q);
void freeQueue(QUEUE q);

#endif

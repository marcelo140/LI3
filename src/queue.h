#ifndef __QUEUE_H__
#define __QUEUE_H__

#define BASE_SIZE 30

#include "generic.h"

typedef struct queue *QUEUE;

QUEUE initQueue(void);
QUEUE enqueue(QUEUE q, void *element);
bool isEmptyQueue(QUEUE q);
void *dequeue(QUEUE q);

#endif

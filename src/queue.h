#ifndef __QUEUE_H__
#define __QUEUE_H__

#define BASE_SIZE 30
typedef struct queue *QUEUE;

QUEUE initQueue(void);
QUEUE enqueue(QUEUE q, void *element);
void *dequeue(QUEUE q);

#endif

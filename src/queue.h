#ifndef __QUEUE_H__
#define __QUEUE_H__

#define BASE_SIZE 30
typedef struct queue *QUEUE;

QUEUE initQueue(void);
QUEUE push(QUEUE q, void *element);
void *depush(QUEUE q);

#endif

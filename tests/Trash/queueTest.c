#include <stdio.h>
#include <string.h>

#define QUEUE_NUM 15

#include "queueTest.h"
#include "../src/queue.h"

static int test_all();

int test_queue() {
	int passed_tests = 0;
	int res;

	res = test_all();
	passed_tests += res;
	printf("queue: %d/%d\n", res, QUEUE_NUM);

	return passed_tests;
}

static int test_all() {
	QUEUE q = initQueue(3);
	char *str;
	int passed_tests = 0;

	if (isEmptyQueue(q))
		passed_tests++;
	else
		return passed_tests;

	q = enqueue(q, "escolhido");
	q = enqueue(q, "centoEquarenta");
	q = enqueue(q, "140");
	
	if (!isEmptyQueue(q))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str, "escolhido"))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str, "centoEquarenta"))
		passed_tests++;
	else
		return passed_tests;

	q = enqueue(q, "nove");
	q = enqueue(q, "cientista");
	q = enqueue(q, "9");
	q = enqueue(q, "candace");

	if (!isEmptyQueue(q))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str,"140"))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str,"nove"))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str,"cientista"))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str,"9"))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str,"candace"))
		passed_tests++;
	else
		return passed_tests;

	if (isEmptyQueue(q))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (str == NULL)
		passed_tests++;
	else
		return passed_tests;

	q = enqueue(q, "again");

	if (!isEmptyQueue(q))
		passed_tests++;
	else
		return passed_tests;

	str = dequeue(q);
	if (!strcmp(str, "again"))
		passed_tests++;
	else
		return passed_tests;

	if (isEmptyQueue(q))
		passed_tests++;
	else
		return passed_tests;

	return passed_tests;
}

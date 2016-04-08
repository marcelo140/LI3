#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/heap.h"

static int test_remove();

#define REMOVE_NUM 6

int test_heap() {
	int res, passed_tests = 0;

	res = test_remove();
	passed_tests += res;
	printf("remove: %d/%d\n", res, REMOVE_NUM);

	return passed_tests;
}

static int test_remove() {
	HEAP h = initHeap(strcmp);
	int passed_tests = 0;
	char *key[3], *content[3];

	h = insertHeap(h, "05", "140");
	h = insertHeap(h, "10", "009");
	h = insertHeap(h, "00", "122");

	key[0] = removeHeap(h, (void **) &content[0]);
	key[1] = removeHeap(h, (void **) &content[1]);
	key[2] = removeHeap(h, (void **) &content[2]);

	if (!strcmp(key[0], "00"))
		passed_tests++;	
	
	if (!strcmp(key[1], "05"))
		passed_tests++;

	if (!strcmp(key[2], "10"))
		passed_tests++;

	if (!strcmp(content[0], "122"))
		passed_tests++;

	if (!strcmp(content[1], "140"))
		passed_tests++;

	if (!strcmp(content[2], "009"))
		passed_tests++;

	return passed_tests;
}

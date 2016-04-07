#include <stdio.h>

#include "avlTest.h"
#include "../src/avl.h"

#define COUNT_NODES_NUM 2
#define IS_EMPTY_NUM 2
#define LOOK_UP_NUM 3

static int test_countNodes();
static int test_isEmpty();
static int test_lookUp();

int test_AVL() {
	int res, passed_tests = 0;

	res = test_countNodes();
	passed_tests += res;
	printf("countNodes: %d/%d\n", res, COUNT_NODES_NUM);

	res = test_isEmpty();
	passed_tests += res;
	printf("isEmptyAVL: %d/%d\n", res, IS_EMPTY_NUM);

	res = test_lookUp();
	passed_tests += res;
	printf("lookUp: %d/%d\n", res, LOOK_UP_NUM);

	return passed_tests;
}

static int test_countNodes() {
	AVL tree = initAVL();
	int passed_tests= 0;

	if (countNodes(tree) == 0)
		passed_tests++;

	tree = insertAVL(tree, "escolhido", NULL);
	tree = insertAVL(tree, "cientista", NULL);

	if (countNodes(tree) == 2)
		passed_tests++;

	freeAVL(tree);
	return passed_tests;
}

static int test_isEmpty() {
	AVL tree = initAVL();
	int passed_tests = 0;

	if (isEmptyAVL(tree))
		passed_tests++;

	tree = insertAVL(tree, "escolhido", NULL);

	if (!isEmptyAVL(tree))
		passed_tests++;

	freeAVL(tree);
	return passed_tests;
}

static int test_lookUp() {
	AVL tree = initAVL();
	int passed_tests = 0;

	tree = insertAVL(tree, "escolhido", NULL);
	tree = insertAVL(tree, "cientista", NULL);

	if (lookUpAVL(tree, "candace") == false)
		passed_tests++;

	tree = insertAVL(tree, "122", NULL);
	tree = insertAVL(tree, "140", NULL);
	tree = insertAVL(tree, "9", NULL);

	if (lookUpAVL(tree, "escolhido") == true)
		passed_tests++;

	if (lookUpAVL(tree, "9") == true)
		passed_tests++;

	freeAVL(tree);
	return passed_tests;
}	

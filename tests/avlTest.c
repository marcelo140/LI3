#include <stdio.h>
#include <string.h>

#include "avlTest.h"
#include "../src/avl.h"

#define COUNT_NODES_NUM 2
#define IS_EMPTY_NUM 2
#define LOOK_UP_NUM 3
#define EQUALS_NUM 4
#define UPDATE_NUM 4

static int test_countNodes();
static int test_isEmpty();
static int test_lookUp();
static int test_equals();
static int test_update();

int test_AVL() {
	int res, passed_tests = 0;

	res = test_countNodes();
	passed_tests += res;
	printf("countNodes: %d/%d\n", res, COUNT_NODES_NUM);

	res = test_isEmpty();
	passed_tests += res;
	printf("isEmpty: %d/%d\n", res, IS_EMPTY_NUM);

	res = test_lookUp();
	passed_tests += res;
	printf("lookUp: %d/%d\n", res, LOOK_UP_NUM);

	res = test_equals();
	passed_tests += res;
	printf("equals: %d/%d\n", res, EQUALS_NUM);

	res = test_update();
	passed_tests += res;
	printf("update: %d/%d\n", res, UPDATE_NUM);

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

static int test_equals() {
	AVL tree1, tree2;
	int passed_tests = 0;

	tree1 = initAVL();
	tree2 = initAVL();

	if (equalsAVL(tree1, tree2))
		passed_tests++;
	
	insertAVL(tree1, "10", NULL);
	insertAVL(tree1, "07", NULL);
	insertAVL(tree1, "15", NULL);
	insertAVL(tree1, "04", NULL);
	insertAVL(tree1, "09", NULL);
	insertAVL(tree1, "12", NULL);
	insertAVL(tree1, "18", NULL);

	if (!equalsAVL(tree1, tree2))
		passed_tests++;

	insertAVL(tree2, "04", NULL);
	insertAVL(tree2, "07", NULL);
	insertAVL(tree2, "09", NULL);

	if (!equalsAVL(tree1, tree2))
		passed_tests++;

	insertAVL(tree2, "10", NULL);
	insertAVL(tree2, "12", NULL);
	insertAVL(tree2, "15", NULL);
	insertAVL(tree2, "18", NULL);

	if (equalsAVL(tree1, tree2))
		passed_tests++;

	freeAVL(tree1);
	freeAVL(tree2);
	return passed_tests;
}

static int test_update() {
	AVL tree = initAVL();
	int passed_tests = 0;
	char *res;

	char str1[] = "escolhido";
	char str2[] = "televisao";
	char str3[] = "terminal";
	char str4[] = "Jacinta";

	tree = insertAVL(tree, "01", str1);
	tree = insertAVL(tree, "02", str2);
	tree = insertAVL(tree, "03", NULL);

	tree = updateAVL(tree, "01", str3);
	tree = updateAVL(tree, "03", str4);
	tree = updateAVL(tree, "19", str2);

	res = getAVLcontent(tree, "01");
	if (!strcmp(res, str3))
		passed_tests++;

	res = getAVLcontent(tree, "02");
	if (!strcmp(res, str2))
		passed_tests++;

	res = getAVLcontent(tree, "03");
	if (!strcmp(res, str4))
		passed_tests++;

	res = getAVLcontent(tree, "19");
	if (res == NULL)
		passed_tests++;

	freeAVL(tree);
	return passed_tests;
}

#include <stdio.h>
#include <string.h>

#include "avlTest.h"
#include "../src/avl.h"

#define COUNT_NODES_NUM 2
#define IS_EMPTY_NUM 2
#define LOOK_UP_NUM 3
#define EQUALS_NUM 4
#define REPLACE_NUM 4
#define SETS_NUM 7

static int test_countNodes();
static int test_isEmpty();
static int test_lookUp();
static int test_equals();
static int test_replace();
static int test_sets();

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

	res = test_replace();
	passed_tests += res;
	printf("replace: %d/%d\n", res, REPLACE_NUM);

	res = test_sets();
	passed_tests += res;
	printf("sets: %d/%d\n", res, SETS_NUM);
	return passed_tests;
}

static int test_countNodes() {
	AVL tree = initAVL(NULL, NULL, NULL, NULL);
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
	AVL tree = initAVL(NULL, NULL, NULL, NULL);
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
	AVL tree = initAVL(NULL, NULL, NULL, NULL);
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

	tree1 = initAVL(NULL, NULL, NULL, NULL);
	tree2 = initAVL(NULL, NULL, NULL, NULL);

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

static int test_replace() {
	AVL tree = initAVL(NULL, NULL, NULL, NULL);
	int passed_tests = 0;
	char *res;

	char str1[] = "escolhido";
	char str2[] = "televisao";
	char str3[] = "terminal";
	char str4[] = "Jacinta";

	tree = insertAVL(tree, "01", str1);
	tree = insertAVL(tree, "02", str2);
	tree = insertAVL(tree, "03", NULL);

	replaceAVL(tree, "01", str3);
	replaceAVL(tree, "03", str4);
	replaceAVL(tree, "19", str2);

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

int test_sets() {
	AVL tree1, tree2;
	HASHSET set1, set2, set3;
	int passed_tests = 0;

	tree1 = initAVL(NULL, NULL, NULL, NULL);
	tree2 = initAVL(NULL, NULL, NULL, NULL);
	set1 = initHashSet(10);
	set2 = initHashSet(10);

	tree1 = insertAVL(tree1, "05", NULL);
	tree1 = insertAVL(tree1, "08", NULL);
	tree1 = insertAVL(tree1, "09", NULL);
	tree1 = insertAVL(tree1, "13", NULL);
	tree1 = insertAVL(tree1, "16", NULL);
	tree1 = insertAVL(tree1, "28", NULL);

	tree2 = insertAVL(tree2, "08", NULL);
	tree2 = insertAVL(tree2, "29", NULL);
	tree2 = insertAVL(tree2, "09", NULL);
	tree2 = insertAVL(tree2, "16", NULL);

	set1 = fillHashSet(set1, tree1);
	set2 = fillHashSet(set2, tree2);
	
	if (getHashSetSize(set1) == 6)
		passed_tests++;

	if (getHashSetSize(set2) == 4)
		passed_tests++;

	set3 = diffHSets(set1, set2);

	if (getHashSetSize(set3) == 4)
		passed_tests++;

	if (!strcmp(getHashSetPos(set3, 1), "13"))
		passed_tests++;

	freeHashSet(set3);
	set3 = unionHSets(set1, set2);

	if (getHashSetSize(set3) == 7)
		passed_tests++;

	if (!strcmp(getHashSetPos(set3, 3), "13"))
		passed_tests++;

	if (!strcmp(getHashSetPos(set3, 5), "28"))
		passed_tests++;

	freeAVL(tree1);
	freeAVL(tree2);
	freeHashSet(set1);
	freeHashSet(set2);
	freeHashSet(set3);
	
	return passed_tests;
}

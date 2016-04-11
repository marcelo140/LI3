#include <stdio.h>
#include <string.h>

#include "avlTest.h"
#include "../src/avl.h"

#define COUNT_NODES_NUM 2
#define IS_EMPTY_NUM 2
#define LOOK_UP_NUM 3
#define EQUALS_NUM 4
#define REPLACE_NUM 4
#define FILL_DATA_SET_NUM 7
#define ADD_DATA_SET_NUM 6
#define DATA_COPY_NUM 3

static int test_countNodes();
static int test_isEmpty();
static int test_lookUp();
static int test_equals();
static int test_replace();
static int test_filldataset();
static int test_addDataSet();
static int test_datacpy();

int test_AVL() {
	int res, passed_tests = 0;

	res = test_countNodes();
	passed_tests += res;
	printf("countNodes: %d/%d\n", res, COUNT_NODES_NUM);

	res = test_isEmpty();
	passed_tests += res;
	printf("isEmpty:    %d/%d\n", res, IS_EMPTY_NUM);

	res = test_lookUp();
	passed_tests += res;
	printf("lookUp:     %d/%d\n", res, LOOK_UP_NUM);

	res = test_equals();
	passed_tests += res;
	printf("equals:     %d/%d\n", res, EQUALS_NUM);

	res = test_replace();
	passed_tests += res;
	printf("replace:    %d/%d\n", res, REPLACE_NUM);

	res = test_filldataset();
	passed_tests += res;
	printf("fill:       %d/%d\n", res, FILL_DATA_SET_NUM);
	
	res = test_addDataSet();
	passed_tests += res;
	printf("addData:    %d/%d\n", res, ADD_DATA_SET_NUM);

	res = test_datacpy();
	passed_tests += res;
	printf("datacopy:   %d/%d\n", res, DATA_COPY_NUM);

	return passed_tests;
}

static int test_countNodes() {
	AVL tree = initAVL(NULL, NULL, NULL, NULL, NULL);
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
	AVL tree = initAVL(NULL, NULL, NULL, NULL, NULL);
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
	AVL tree = initAVL(NULL, NULL, NULL, NULL, NULL);
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

	tree1 = initAVL(NULL, NULL, NULL, NULL, NULL);
	tree2 = initAVL(NULL, NULL, NULL, NULL, NULL);

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
	AVL tree = initAVL(NULL, NULL, NULL, NULL, NULL);
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

int test_filldataset() {
	DATASET set = initDataSet(5);
	AVL tree = initAVL(NULL, NULL, NULL, NULL, NULL);
	int i, passed_tests = 0;

	tree = insertAVL(tree, "140", "escolhido");
	tree = insertAVL(tree, "122", "candace");
	tree = insertAVL(tree, "009", "cientista");	
	tree = insertAVL(tree, "003", "lord");
	tree = insertAVL(tree, "004", "ryu");
	tree = insertAVL(tree, "008", "squirtle");
	tree = insertAVL(tree, "011", "gajo");
	tree = insertAVL(tree, "016", "gajo2");
	tree = insertAVL(tree, "017", "eletro");

	if (getDataSetSize(set) == 0)
		passed_tests++;
	else
		return passed_tests;

	set = fillDataSet(set, tree);
	
	if (getDataSetSize(set) == 9)
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getHashPos(set, 2), "008"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getHashPos(set, 8), "140"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getDataPos(set, 1), "ryu"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getDataPos(set, 6), "eletro"))
		passed_tests++;
	else
		return passed_tests;

	for(i = 1; i > 8; i++)
		if (strcmp(getHashPos(set, i), getHashPos(set, i-1)) < 0)
			return passed_tests;

	passed_tests++;

	return passed_tests;
}


static int test_addDataSet(){
	AVL tree1, tree2, tree3;
	DATASET set;
	int i, passed_tests = 0;

	tree1 = initAVL(NULL, NULL, NULL, NULL, NULL);
	tree2 = initAVL(NULL, NULL, NULL, NULL, NULL);
	tree3 = initAVL(NULL, NULL, NULL, NULL, NULL);
	set = initDataSet(5);
	
	tree1 = insertAVL(tree1, "19", "tyty");
	tree1 = insertAVL(tree1, "30", "paquitos");
	tree1 = insertAVL(tree1, "31", "trintaEum");

	tree2 = insertAVL(tree2, "32", "broken");
	tree2 = insertAVL(tree2, "33", "gaijo");
	tree2 = insertAVL(tree2, "35", "nemcomento");

	tree3 = insertAVL(tree3, "36", "nikita");
	tree3 = insertAVL(tree3, "37", "shaman");
	tree3 = insertAVL(tree3, "38", "shimbch");

	set = addDataSet(set, tree1);
	set = addDataSet(set, tree2);
	set = addDataSet(set, tree3);

	if (getDataSetSize(set) == 9)
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getHashPos(set, 2), "31"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getHashPos(set, 5), "35"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getDataPos(set, 6), "nikita"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getDataPos(set, 8), "shimbch"))
		passed_tests++;
	else
		return passed_tests;

	for(i = 1; i > 8; i++)
		if (strcmp(getHashPos(set, i), getHashPos(set, i-1)) < 0)
			return passed_tests;

	passed_tests++;

	return passed_tests;
}

int test_datacpy() {
	int passed_tests = 0;
	DATASET set1, set2;
	AVL tree;

	tree = initAVL(NULL, NULL, NULL, NULL, NULL);
	set1 = initDataSet(3);
	set2 = initDataSet(2);

	tree = insertAVL(tree, "140", "eu");
	tree = insertAVL(tree, "009", "ele");
	tree = insertAVL(tree, "122", "ela");
	
	set1 = addDataSet(set1, tree);
	set2 = datacpy(set2, set1, 1);
	set2 = datacpy(set2, set1, 0);
	set2 = datacpy(set2, set1, 2);

	if (!strcmp(getHashPos(set2, 1), "009"))
		passed_tests++;
	else
		return passed_tests;

	if (!strcmp(getDataPos(set2, 0), "ela"))
		passed_tests++;
	else
		return passed_tests;
	
	if (!strcmp(getHashPos(set2, 2), "140"))
		passed_tests++;
	else
		return passed_tests;
		
	return passed_tests;
}

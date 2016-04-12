#include <stdio.h>

#include "catalogTest.h"
#include "avlTest.h"
#include "salesTest.h"
#include "heapTest.h"
#include "hashTTest.h"

void printHeader(const char *str);

int main () {
	printHeader("TESTING AVL");	
	test_AVL();

	printHeader("TESTING CATALOG");
	test_Cat();

	printHeader("TESTING HEAP");
	test_heap();

	printHeader("TESTING SALES");
	test_sales();

	return 0;
}

void printHeader(const char *str){
	putchar('\n');
	printf("##### %s ########################\n", str);
}

#include <stdio.h>

#include "catalogTest.h"
#include "avlTest.h"
#include "heapTest.h"

void printHeader(const char *str);

int main () {
	printHeader("TESTING AVL");	
	test_AVL();

	putchar('\n');

	printHeader("TESTING CATALOG");
	test_Cat();

	putchar('\n');

	printHeader("TESTING HEAP");
	test_heap();

	return 0;
}

void printHeader(const char *str){
	printf("##### %s ########################\n", str);
}

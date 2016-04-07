#include <stdio.h>

#include "catalogTest.h"
#include "avlTest.h"

void printHeader(const char *str);

int main () {
	printHeader("TESTING AVL");	
	test_AVL();

	putchar('\n');

	printHeader("TESTING CATALOG");
	test_Cat();
	return 0;
}

void printHeader(const char *str){
	printf("##### %s ########################\n", str);
}

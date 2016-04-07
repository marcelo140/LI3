#include <stdio.h>

#include "avlTest.h"

void printHeader(const char *str);

int main () {
	printHeader("TESTING AVL");	
	test_AVL();
	return 0;
}

void printHeader(const char *str){
	printf("##### %s ########################\n", str);
}

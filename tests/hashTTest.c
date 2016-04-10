#include <stdlib.h>
#include <stdio.h>
#include "hashTTest.h"

#define NUM_TESTS 2

int test_hashT() {

	int passed_tests = 0;
	HASHTABLE ht = initHashTable();
	SALE s1 = initSale(), s2 = initSale();
	PRODUCT p1 = toProduct("Tea"), p2 = toProduct("aeT");
	CLIENT c = toClient("Maria");
	HASH h;

	double valueD;
	int valueI;

	s1 = updateSale(s1, p1, c, 12.03, 2, 5, 3, MODE_N );
	s2 = updateSale(s2, p2, c, 5.01, 10, 3, 2, MODE_P );	


	ht = insertHashT(ht, NULL, s1);
	ht = insertHashT(ht, NULL, s2);

	h = createHash(ht, fromProduct(p1));

	if ((valueD = getHashBilled(ht, h, 4, 2, MODE_N)) == 24.06) passed_tests++;
	freeHash(h);

	h = createHash(ht, fromProduct(p2));
	if ((valueI = getHashQuantity(ht, h, 2, 1, MODE_P)) == 10) passed_tests++;	
	else printf("Chumbou: %d\n", valueI); 

	printf("Passou %d/%d\n", passed_tests, NUM_TESTS);

	freeHash(h);
	freeHashTable(ht);
/*	freeSale(s1, M_CONTENT);
	freeSale(s2, M_CONTENT); */
	free(s1);
	free(s2);;
	freeProduct(p1);
	freeProduct(p2);
	freeClient(c);
	return passed_tests;
}

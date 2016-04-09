#include <stdio.h>

#include "../src/revenue.h"
#include "revenueTest.h"

#define TESTS_NUM 5

int test_revenue() {

	int passed_tests = 0;

	REVENUE r = initRevenue();

	if (getBilled(r, 3, MODE_N) == 0 && getQuantity(r, 8, MODE_P) == 0) passed_tests++; 

	r = updateRevenue(r, 2, MODE_P, 24.5, 3);
	r = updateRevenue(r, 2, MODE_N, 14.23, 10);
	r = updateRevenue(r, 2, MODE_P, 10, 2);

	if (getBilled(r, 2, MODE_P) == 14.5 && getQuantity(r, 2, MODE_P) == 5) passed_tests++;

	r = addBilled(r, 2, MODE_N, 14.20);
	
	if (getBilled(r, 2, MODE_N) == 28.43 && getQuantity(r, 2, MODE_N) == 10) passed_tests++;

	r = addQuantity(r, 11, MODE_P, 100);
	r = addBilled(r, 0, MODE_N, 30.1242);

	if (getBilled(r, 11, MODE_P) == 0 && getQuantity(r, 11, MODE_P) == 100) passed_tests++;
	if (getBilled(r, 0, MODE_N) == 30.1242 && getQuantity(r, 0, MODE_N) == 0) passed_tests++;	

	printf("REVENUE: %d/%d", passed_tests, TESTS_NUM);

	return passed_tests;
}

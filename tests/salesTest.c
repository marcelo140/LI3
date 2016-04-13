#include <stdio.h>

#include "../src/sales.h"
#include "salesTest.h"
#include "../src/products.h"
#include "../src/clients.h"

#define REVENUE_NUM 9

static int test_revenue();

int test_sales() {
	int res, passed_tests = 0;
	
	res = test_revenue();
	passed_tests += res;
	printf("REVENUE: %d/%d\n", res, REVENUE_NUM);

	return passed_tests;
}

static int test_revenue() {
	int resQ, normalQ, promoQ, passed_tests = 0;
	double resB, normalB, promoB;
	SALE s = initSale();
	REVENUE r = initRevenue();

	if (getBilled(r, 3, 1, MODE_N) == 0 && getQuantity(r, 8, 1, MODE_P) == 0) 
		passed_tests++;
	else
		return passed_tests;

	r = updateRevenue(r, 5, 2, MODE_N, 10.0, 1);
	r = updateRevenue(r, 5, 2, MODE_P, 24.5, 3);
	r = updateRevenue(r, 1, 0, MODE_N, 14.23, 10);
	r = updateRevenue(r, 2, 0, MODE_P, 10, 2);

	if (getBilled(r, 5, 2, MODE_P) == 73.5 && getQuantity(r, 2, 0, MODE_P) == 2) 
		passed_tests++;
	else
		return passed_tests;

	r = addBilled(r, 5, 2, MODE_N, 14.20);
	
	if (getBilled(r, 5, 2, MODE_N) == 24.2 && getQuantity(r, 1, 0, MODE_N) == 10) 
		passed_tests++;
	else
		return passed_tests;

	r = addQuantity(r, 11, 0, MODE_P, 100);
	r = addBilled(r, 0, 0, MODE_N, 30.1242);

	if (getBilled(r, 11, 0, MODE_P) == 0 && getQuantity(r, 11, 0, MODE_P) == 100) 
		passed_tests++;
	else
		return passed_tests;

	if (getBilled(r, 0, 0, MODE_N) == 30.1242 && getQuantity(r, 0, 0, MODE_N) == 0) 
		passed_tests++;
	else
		return passed_tests;

	resQ = getBranchQuant(r, 2, &normalQ, &promoQ);

	if (normalQ == 1 && promoQ == 3 && resQ == 4)
		passed_tests++;
	else
		return passed_tests;

	resB = getBranchBilled(r, 2, &normalB, &promoB);

	if (normalB == 24.2 && promoB == 73.5 && resB == normalB+promoB)
		passed_tests++;
	else
		return passed_tests;

	s = updateSale(s, toProduct("QZ1184"), toClient("A1183"), 9.85, 3, 2, 2, MODE_N);	
	r = addSale(r, s);

	resQ = getMonthQuant(r, 2, &normalQ, &promoQ);
	resB = getMonthBilled(r, 5, &normalB, &promoB);

	if (normalB == 24.2 && promoB == 73.5 && resB == 97.7)
		passed_tests++;
	else
		return passed_tests;

	if (normalQ == 3 && promoQ == 2 && resQ == 5)
		passed_tests++;
	else
		return passed_tests;

	return passed_tests;
}

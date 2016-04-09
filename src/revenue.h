#ifndef __REVENUES_H__
#define __REVENUES_H__

#define MODE_N 0
#define MODE_P 1

#include "sales.h"

typedef struct revenue *REVENUE;

REVENUE initRevenue();
REVENUE addSale(REVENUE r, SALE s);
REVENUE updateRevenue(REVENUE r, int month, int branch, int MODE, double billed, int quantity); 
REVENUE addBilled(REVENUE r, int month, int branch, int MODE, double value);
REVENUE addQuantity(REVENUE r, int month, int branch, int MODE, int value);
double getBilled(REVENUE r, int month, int branch, int MODE);
int getQuantity(REVENUE r, int month, int branch, int MODE);
void freeRevenue(REVENUE r);

#endif

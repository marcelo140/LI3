#ifndef __REVENUES_H__
#define __REVENUES_H__

#define MODE_N 0
#define MODE_P 1

typedef struct revenue *REVENUE;

REVENUE initRevenue();  
REVENUE updateRevenue(REVENUE r, int month, int MODE, double billed, int quantity); 
REVENUE addBilled(REVENUE r, int month, int MODE, double value);
REVENUE addQuantity(REVENUE r, int month, int MODE, int value);
double getBilled(REVENUE r, int month, int MODE);
int getQuantity(REVENUE r, int month, int MODE);
void freeRevenue(REVENUE r);

#endif

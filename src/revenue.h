#ifndef __REVENUES_H__
#define __REVENUES_H__

#define MODE_N 0
#define MODE_P 1

typedef struct revenue *REVENUE;

REVENUE initTransaction ();  
REVENUE updateRevenue(REVENUE v, int month, int MODE, double billed, int quantity); 
REVENUE setBilled(REVENUE t, int month, int MODE, double value);
REVENUE setQuantity(REVENUE t, int month, int MODE, int value);
double getBilled(REVENUE v, int month, int MODE);
int getQuantity(REVENUE v, int month, int MODE);
void freeTransaction(REVENUE t);

#endif

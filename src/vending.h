#ifndef __VENDINGS_H__
#define __VENDINGS_H__

#define MODE_N 0
#define MODE_P 1

typedef struct vending *VENDING;

VENDING initTransaction ();  
VENDING updateVending(VENDING v, int month, int MODE, double billed, int quantity); 
VENDING setBilled(VENDING t, int month, int MODE, double value);
VENDING setQuantity(VENDING t, int month, int MODE, int value);
double getBilled(VENDING v, int month, int MODE);
int getQuantity(VENDING v, int month, int MODE);
void freeTransaction(VENDING t);

#endif

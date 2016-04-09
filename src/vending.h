#ifndef __VENDINGS_H__
#define __VENDINGS_H__

#define MODE_N 0
#define MODE_P 1

typedef struct vending *VENDING;

VENDING initTransaction ();
VENDING setBilled(VENDING t, int month, int MODE, int value);
VENDING setQuantity(VENDING t, int month, int MODE, int value);
void freeTransaction(VENDING t);

#endif

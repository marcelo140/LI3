#ifndef __TRANSACTIONS_H__
#define __TRANSACTIONS_H__

#define MODE_N 0
#define MODE_P 1

typedef struct transaction *TRANSACTION;

TRANSACTION initTransaction ();
TRANSACTION setBilled(TRANSACTION t, int month, int MODE, int value);
TRANSACTION setQuantity(TRANSACTION t, int month, int MODE, int value);
void freeTransaction(TRANSACTION t);

#endif

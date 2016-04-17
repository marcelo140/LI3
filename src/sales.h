#ifndef __SALES__
#define __SALES__

#include "generic.h"
#include "clients.h"
#include "products.h"

#define MODE_N 0
#define MODE_P 1

typedef struct sale    *SALE;
typedef struct revenue *REVENUE;

SALE    initSale    ();

SALE    readSale      (SALE s, PRODUCT p, CLIENT c, char *line);
SALE    updateSale    (SALE s, PRODUCT p, CLIENT c,
                       double price, int quant, int month, int branch, int mode);


bool isSale     (SALE sale, PRODUCTCAT prodCat, CLIENTCAT clientCat);

PRODUCT getProduct (SALE s);
CLIENT  getClient  (SALE s);
double  getPrice   (SALE s);
int     getQuant   (SALE s);
int     getMonth   (SALE s);
int     getBranch  (SALE s);
int     getMode    (SALE s);

/*double  getBilled       (REVENUE r, int month,  int branch, int MODE);*/

/*int     getQuantity     (REVENUE r, int month, int branch, int MODE);*/

void freeSale    (SALE s);

#endif

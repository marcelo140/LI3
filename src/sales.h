#ifndef __SALES_H__
#define __SALES_H__

#include "generic.h"
#include "clients.h"
#include "products.h"

#define MODE_N 0
#define MODE_P 1

#define M_CONTENT 0
#define M_TOTAL 1

typedef struct sale    *SALE;
typedef struct revenue *REVENUE;

SALE    initSale    ();
REVENUE initRevenue ();

SALE    updateSale    (SALE s, PRODUCT p, CLIENT c, double price, int quant, int month, int branch, int mode);
SALE    readSale      (SALE s, PRODUCT p, CLIENT c, char *line);
REVENUE addSale       (REVENUE r, SALE s);
REVENUE updateRevenue (REVENUE r, int month, int branch, int MODE, double billed, int quantity); 
REVENUE addBilled     (REVENUE r, int month, int branch, int MODE, double value);
REVENUE addQuantity   (REVENUE r, int month, int branch, int MODE, int value);

bool isEmptyRev (REVENUE r);
bool isSale     (SALE sale, PRODUCTCAT prodCat, CLIENTCAT clientCat);

PRODUCT getProduct (SALE s);
CLIENT  getClient  (SALE s);
double  getPrice   (SALE s);
int     getQuant   (SALE s);
int     getMonth   (SALE s);
int     getBranch  (SALE s);
int     getMode    (SALE s);

double  getMonthBilled  (REVENUE r, int month,  double *normal, double *promo);
double  getBranchBilled (REVENUE r, int branch, double *normal, double *promo);
double  getBilled       (REVENUE r, int month,  int branch,  int MODE);

int     getBranchQuant  (REVENUE r, int branch, int *normal, int *promo);
int     getMonthQuant   (REVENUE r, int month,  int *normal, int *promo);
int     getQuantity     (REVENUE r, int month, int branch, int MODE);

void freeSale    (SALE s);
void freeRevenue (REVENUE r);

#endif

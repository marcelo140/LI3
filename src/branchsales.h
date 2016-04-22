#ifndef __BRANCHSALES__
#define __BRANCHSALES__

#include "products.h"
#include "clients.h"
#include "sales.h"

typedef struct branchsales *BRANCHSALES;
typedef struct product_data *PRODUCTDATA;

BRANCHSALES initBranchSales();

BRANCHSALES fillBranchSales(BRANCHSALES bs, CLIENTCAT cc, PRODUCTCAT pc);

BRANCHSALES addSaleToBranch(BRANCHSALES bs, SALE s);

int* getClientQuantByMonth(BRANCHSALES bs, CLIENT c);

SET getClientsWhoBought(BRANCHSALES bs);

SET getClientsWhoHaveNotBought(BRANCHSALES bs);

void getClientsByProduct(BRANCHSALES bs, PRODUCT prod, SET *normal, SET *clients);

SET getProductsByClient(BRANCHSALES bs, CLIENT c);

SET listProductsByQuant(BRANCHSALES bs);

int getClientsFromData(PRODUCTDATA pd);

int getQuantFromData(PRODUCTDATA pd);

void sortProductListByQuant(SET productList, int month);

void sortProductListByBilled(SET productList);

void freeProductData(PRODUCTDATA pd);

void freeBranchSales(BRANCHSALES bs);

#endif

#ifndef __BRANCHSALES__
#define __BRANCHSALES__

#include "products.h"
#include "clients.h"
#include "sales.h"

typedef struct branchsales *BRANCHSALES;

BRANCHSALES initBranchSales();

BRANCHSALES fillBranchSales(BRANCHSALES bs, CLIENTCAT cc, PRODUCTCAT pc);

BRANCHSALES addSaleToBranch(BRANCHSALES bs, SALE s);

int* getClientQuantByMonth(BRANCHSALES bs, CLIENT c);

LIST getClientsWhoBought(BRANCHSALES bs);

void getClientsByProduct(BRANCHSALES bs, PRODUCT prod, LIST *normal, LIST *clients);

LIST getProductsByClient(BRANCHSALES bs, CLIENT c);

void sortProductListByQuant(LIST productList, int month);

void sortProductListByBilled(LIST productList);

void freeBranchSales(BRANCHSALES bs);

#endif

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

void freeBranchSales(BRANCHSALES bs);

#endif

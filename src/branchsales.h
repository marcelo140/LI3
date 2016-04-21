#ifndef __BRANCHSALES__
#define __BRANCHSALES__

#include "products.h"
#include "clients.h"

typedef struct branchsales *BRANCHSALES;

BRANCHSALES initBranchSales();

BRANCHSALES fillBranchSales(BRANCHSALES bs, CLIENTCAT cc, PRODUCTCAT pc);

void freeBranchSales(BRANCHSALES bs);

#endif

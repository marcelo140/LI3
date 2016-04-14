#ifndef __VENDAS_FILIAL__
#define __VENDAS_FILIAL__

#include "sales.h"
#include "generic.h"

typedef struct branch *BRANCHSALES;

BRANCHSALES initBranchSales();
BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s);

void freeBranchSale(BRANCHSALES bs);

#endif

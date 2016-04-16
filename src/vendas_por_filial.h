#ifndef __VENDAS_FILIAL__
#define __VENDAS_FILIAL__

#include "sales.h"
#include "generic.h"

typedef struct branch      *BRANCHSALES;
typedef struct clientlist  *CLIENTLIST;
typedef struct productlist *PRODUCTLIST;

typedef char SORT_MODE;

#define BY_QUANT    0
#define BY_EXPENSES 1

BRANCHSALES initBranchSales (CLIENTCAT client);
BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s);

CLIENTLIST getClientsWhoBought   (BRANCHSALES bs);
CLIENTLIST getClientsWhoDidntBuy (BRANCHSALES bs);
void filterClientByProduct       (BRANCHSALES bs, PRODUCT prod, CLIENTLIST n, CLIENTLIST p);

PRODUCTLIST filterProductByClient(BRANCHSALES bs, CLIENT c);

int*    getClientQuant    (BRANCHSALES bs, CLIENT c);
double* getClientExpenses (BRANCHSALES bs, CLIENT c);

void freeBranchSale  (BRANCHSALES bs);
void freeClientList  (CLIENTLIST cl);
void freeProductList (PRODUCTLIST pl);

CLIENTLIST  sortClientList  (CLIENTLIST  cl, SORT_MODE mode);
PRODUCTLIST sortProductList (PRODUCTLIST pl, SORT_MODE mode);

#endif

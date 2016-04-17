#ifndef __QUERIES_H__
#define __QUERIES_H__

#include "interpreter.h"
#include "fatglobal.h"
#include "vendas_por_filial.h"

PRINTSET query2(PRODUCTCAT pcat, char index);
PRINTSET query3(FATGLOBAL fat, PRODUCT product, int month);
PRINTSET query5(BRANCHSALES bs, CLIENT client);
PRINTSET query8(BRANCHSALES bs, PRODUCT product);
PRINTSET query10(FATGLOBAL fat, BRANCHSALES bs, int n); 

#endif

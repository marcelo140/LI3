#include <stdio.h>
#include <time.h>

#include "interpreter.h"
#include "dataloader.h"
#include "clients.h"
#include "products.h"

int main() {
	FATGLOBAL fat;
	BRANCHSALES branchSales[3];
	CLIENTCAT clientCat;
	PRODUCTCAT productCat;
	int i;

	clientCat = initClientCat();
	productCat = initProductCat();
	for(i = 0; i < 3; i++)
		branchSales[i] = initBranchSales(clientCat);
	fat = initFat(productCat);

	interpreter(branchSales, fat, productCat, clientCat);

	return 0;
}

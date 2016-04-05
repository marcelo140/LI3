#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

struct faturacao {
	CATALOG l;
};

struct fatProduct {
	double billed[MONTHS][BRANCHES][NP];
};

FATGLOBAL initFat() {
		FATGLOBAL fat = malloc(sizeof (*fat));
		fat->l = initCatalog();

		return fat;
}

bool isEmptyFat(FATGLOBAL f) {
	return (f==NULL);
}

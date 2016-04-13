#include "vendas_por_filial.h"
#include "catalog.h"
#include "heap.h"

struct filial {
	CATALOG clients;
};

struct month_list {
	struct product_list *months[12];	
};

struct product_list {
	HEAP sales;
	double billed;
	int quant;
};

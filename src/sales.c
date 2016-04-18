#include <stdlib.h>
#include <string.h>

#include "sales.h"

#define MONTHS 12
#define BRANCHES 3
#define PROMO 2

struct sale {
	PRODUCT prod; 
	CLIENT client;
	double price; 
	int quantity; 
	int month;     
	int branch;     
	int mode;       
};

static SALE updateSale (SALE s, PRODUCT p, CLIENT c, double price, int quant, int month,
                        int branch, int mode);

SALE initSale() {
	return malloc(sizeof(struct sale));
}

SALE readSale(SALE s, PRODUCT p, CLIENT c, char *line) {
	char *token;
	double price;
	int quant, month, branch, mode;

	token = strtok(line, " ");
	p = writeProduct(p, token);
	
	token = strtok(NULL, " ");
	price = atof(token);

	token = strtok(NULL, " ");
	quant = atoi(token);

	token = strtok(NULL, " ");
	mode = strcmp(token, "N") ? 1 : 0;

	token = strtok(NULL, " ");
	c = writeClient(c, token);

	token = strtok(NULL, " ");
	month = atoi(token);

	token = strtok(NULL, " ");
	branch = atoi(token);

	return updateSale(s, p, c, price, quant, month-1, branch-1, mode);	
}

bool isSale(SALE sale, PRODUCTCAT prodCat, CLIENTCAT clientCat) {
	return (lookUpProduct(prodCat, sale->prod) && 
	  	 lookUpClient(clientCat, sale->client));
}

PRODUCT getProduct(SALE s) {
	return s->prod;
}

CLIENT getClient(SALE s) {
	return s->client;
}

double getPrice(SALE s) {
	return s->price;
}

int getQuant(SALE s) {
	return s->quantity;
}

int getMonth(SALE s) {
	return s->month;
}

int getBranch(SALE s) {
	return s->branch;
}

int getMode(SALE s) {
	return s->mode;
}

void freeSale(SALE s) {
	free(s);
}

static SALE updateSale(SALE s, PRODUCT p, CLIENT c, double price, int quant, int month, 
                                                                  int branch, int mode)
 {
	s->prod = p;
	s->client = c;
	s->price = price;
	s->quantity = quant;
	s->month = month;
	s->branch = branch;
	s->mode = mode;
	
	return s;
}

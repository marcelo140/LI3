#include <stdlib.h>
#include <string.h>

#include "sales.h"

#define MONTHS 12
#define BRANCHES 3
#define PROMO 2

/**
 * Estrutura usada para representar vendas;
 */
struct sale {
	PRODUCT prod;   /** Produto comprado */
	CLIENT client;  /** Cliente que efetuou a compra */
	double price;   /** Preço da compra */
	int quantity;   /** Quantidade comprada */
	int month;      /** Mês em que a compra foi efetuada */
	int branch;     /** Filial onde foi efetuada a venda */
	int mode;       /** Promoção N (Normal) ou P (Promoção). */
};

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

SALE updateSale(SALE s, PRODUCT p, CLIENT c, double price, int quant, int month, int branch, int mode) {

	s->prod = p;
	s->client = c;
	s->price = price;
	s->quantity = quant;
	s->month = month;
	s->branch = branch;
	s->mode = mode;
	
	return s;
}


/**
 * Adicionar ao total faturado o valor faturado num dado mes num dado modo.
 * @param r REVENUE a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return REVENUE atualizada
 */

/*
REVENUE addBilled(REVENUE r, int month, int branch, int MODE, double value) {
	r->billed[month][branch][MODE] += value;
	r->totalBilled += value;

	return r;	
}
*/

/**
 * Adicionar à quantidade total o valor num dado mes num dado modo.
 * @param r REVENUE a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return REVENUE atualizada
 */
/*
REVENUE addQuantity(REVENUE r, int month, int branch, int MODE, int value) {
	r->quantity[month][branch][MODE] += value;
	r->totalQuant += value;

	return r;	
}
*/
/** 
 * Adiciona uma nova faturação e uma nova quantidade à REVENUE
 * @param r REVENUE a modificar
 * @param month Mês em questão
 * @param billed Faturação a adicionar
 * @param quantity Quantidade a adicionar
 * @return REVENUE alterada 
 */
/*
REVENUE updateRevenue(REVENUE r, int month, int branch, int MODE, double price, int quant){
	double billed = quant*price;

	r->billed[month][branch][MODE]   += billed;
	r->quantity[month][branch][MODE] += quant;
	r->totalBilled += billed;
	r->totalQuant += quant;

	return r;
}
*/
/**
 * Verifica se uma SALE é válida.
 * @param sale SALE a verificar
 * @param prodCat Catálogo de Produtos
 * @param clientCat Cátalogo de Clientes
 * @return true se for válida, false caso contrário
 */
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

/**
 * Devolve o total faturado num dado mês numa dada promoção
 * @param r REVENUE
 * @param month Mês em questão
 * @param MODE Modo da promoção
 * @return O total faturado
 */
/*
double getBilled(REVENUE r, int month, int branch, int MODE) {
	return r->billed[month][branch][MODE]; 
}
*/


/**
 * Devolve a quantidade total vendida num dado mês num dada promoção
 * @param r REVENUE
 * @param month Mês em questão
 * @param MODE Modo da promoção
 * @return A quantidade total vendida
 */
/*
int getQuantity(REVENUE r, int month, int branch, int MODE) {
	return r->quantity[month][branch][MODE];
}
*/
/**
 * Liberta o espaço ocupado pela REVENUE  
 * @param r REVENUE a libertar
 */

/**
 * Liberta o espaço ocupado pela SALE s
 * @param s SALE a libertar
 */
void freeSale(SALE s) {
	free(s);
}


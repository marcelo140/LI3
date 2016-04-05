#include <stdlib.h>
#include "sales.h"

/**
 * Estrutura usada para representar vendas;
 */
struct sale {
	PRODUCT prod; /** Produto comprado */
	CLIENT client; /** Cliente que efetuou a compra */
	double price; /** Preço da compra */
	int quantity; /** Quantidade comprada */
	int month; /** Mês em que a compra foi efetuada */
	int branch; /** Filial onde foi efetuada a venda */
	int mode; /** Promoção N (Normal) ou P (Promoção). */
};

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

/**
 * Cria um SALE novo.
 * @param p Produto
 * @param c Client
 * @param price Preço da venda
 * @param quant Quantidade comprada
 * @param month Mês da compra
 * @param branch Filial onde foi efetuada a compra
 * @param mode Modo de promoção N ou P
 * @return nova SALE
 */
SALE toSale(PRODUCT p, CLIENT c, double price, int quant, int month, int branch, int mode) {

	SALE s = malloc (sizeof(*s));

	s->prod = cloneProduct(p);
	s->client = cloneClient(c);
	s->price = price;
	s->quantity = quant;
	s->month = month;
	s->branch = branch;
	s->mode = mode;
	
	return s;
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

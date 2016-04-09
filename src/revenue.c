#include <stdlib.h>
#include "revenue.h"

#define MONTHS 12
#define PROMO 2

struct revenue{
	double billed[MONTHS][PROMO];
	int quantity[MONTHS][PROMO];
};

/**
 * Inicializa a REVENUE e coloca todos os seus valores a 0;
 * @return nova REVENUE
 */
REVENUE initRevenue() {
	REVENUE new = malloc (sizeof(*new));
	int i;

	for (i=0; i < MONTHS; i++) {
		new->billed[i][0] = 0;
		new->billed[i][1] = 0;
		new->quantity[i][0] = 0;
		new->quantity[i][1] = 0;
	}

	return new;
}

/** 
 * Adiciona uma nova faturação e uma nova quantidade à REVENUE
 * @param r REVENUE a modificar
 * @param month Mês em questão
 * @param billed Faturação a adicionar
 * @param quantity Quantidade a adicionar
 * @return REVENUE alterada 
 */
REVENUE updateRevenue(REVENUE r, int month, int MODE, double billed, int quantity) {
	r->billed[month][MODE] += billed;
	r->quantity[month][MODE] += quantity;

	return r;
}

/**
 * Adicionar ao total faturado o valor faturado num dado mes num dado modo.
 * @param r REVENUE a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return REVENUE atualizada
 */
REVENUE addBilled(REVENUE r, int month, int MODE, double value) {
	r->billed[month][MODE] += value;

	return r;	
}

/**
 * Adicionar à quantidade total o valor num dado mes num dado modo.
 * @param r REVENUE a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return REVENUE atualizada
 */
REVENUE addQuantity(REVENUE r, int month, int MODE, int value) {
	r->quantity[month][MODE] += value;

	return r;	
}

/**
 * Devolve o total faturado num dado mês numa dada promoção
 * @param r REVENUE
 * @param month Mês em questão
 * @param MODE Modo da promoção
 * @return O total faturado
 */
double getBilled(REVENUE r, int month, int MODE) {
	return r->billed[month][MODE]; 
}

/**
 * Devolve a quantidade total vendida num dado mês num dada promoção
 * @param r REVENUE
 * @param month Mês em questão
 * @param MODE Modo da promoção
 * @return A quantidade total vendida
 */
int getQuantity(REVENUE r, int month, int MODE) {
	return r->quantity[month][MODE];
}

/**
 * Liberta o espaço ocupado pela REVENUE  
 * @param r REVENUE a libertar
 */
void freeRevenue(REVENUE r) {
	free(r);
}

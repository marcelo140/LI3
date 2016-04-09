#include <stdlib.h>
#include "vending.h"

#define MONTHS 12
#define PROMO 2

struct vending{
	double billed[MONTHS][PROMO];
	int quantity[MONTHS][PROMO];
};

/**
 * Inicializa a VENDING e coloca todos os seus valores a 0;
 * @return nova VENDING
 */
VENDING initVending() {
	VENDING new = malloc (sizeof(*new));
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
 * Adicionar ao total faturado o valor faturado num dado mes num dado modo.
 * @param v VENDING a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return VENDING atualizada
 */
VENDING addBilled(VENDING v, int month, int MODE, int value) {
	v->billed[month][MODE] += value;

	return v;	
}

/**
 * Adicionar à quantidade total o valor num dado mes num dado modo.
 * @param v VENDING a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return VENDING atualizada
 */
VENDING addQuantity(VENDING v, int month, int MODE, int value) {
	v->quantity[month][MODE] += value;

	return v;	
}

/**
 * Devolve o total faturado num dado mês numa dada promoção
 * @param v VENDING
 * @param month Mês em questão
 * @param MODE Modo da promoção
 * @return O total faturado
 */
double getBilled(VENDING v, int month, int MODE) {
	return v->billed[month][MODE]; 
}

/**
 * Devolve a quantidade total vendida num dado mês num dada promoção
 * @param v VENDING
 * @param month Mês em questão
 * @param MODE Modo da promoção
 * @return A quantidade total vendida
 */
int getQuantity(VENDING v, int month, int MODE) {
	return v->quantity[month][MODE];
}

/**
 * Liberta o espaço ocupado pela VENDING t
 * @param t VENDING a libertar
 */
void freeTransaction(VENDING v) {
	free(v);
}

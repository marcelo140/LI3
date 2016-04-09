#include <stdlib.h>
#include "vending.h"

#define MONTHS 12
#define NP 2

struct vending{
	double billed[MONTHS][NP];
	int quantity[MONTHS][NP];
};

/**
 * Inicializa a VENDING e coloca todos os seus valores a 0;
 * @return nova VENDING
 */
VENDING initTransaction () {
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
 * @param t VENDING a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return VENDING atualizada
 */
VENDING addBilled(VENDING t, int month, int MODE, int value) {
	t->billed[month][MODE] += value;

	return t;	
}

/**
 * Adicionar à quantidade total o valor num dado mes num dado modo.
 * @param t VENDING a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return VENDING atualizada
 */

VENDING addQuantity(VENDING t, int month, int MODE, int value) {
	t->quantity[month][MODE] += value;

	return t;	
}

/**
 * Liberta o espaço ocupado pela VENDING t
 * @param t VENDING a libertar
 */
void freeTransaction(VENDING t) {
	free(t);
}

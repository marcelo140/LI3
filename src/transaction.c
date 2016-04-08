#include <stdlib.h>
#include "transaction.h"

#define MONTHS 12
#define NP 2

struct transaction {
	double billed[MONTHS][NP];
	int quantity[MONTHS][NP];
};

/**
 * Inicializa a TRANSACTION e coloca todos os seus valores a 0;
 * @return nova TRANSACTION
 */
TRANSACTION initTransaction () {
	TRANSACTION new = malloc (sizeof(*new));
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
 * @param t TRANSACTION a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return TRANSACTION atualizada
 */
TRANSACTION addBilled(TRANSACTION t, int month, int MODE, int value) {
	t->billed[month][MODE] += value;

	return t;	
}

/**
 * Adicionar à quantidade total o valor num dado mes num dado modo.
 * @param t TRANSACTION a adicionar
 * @param m Mês da venda
 * @param MODE MODE_N caso seja normal, MODE_P caso tenha promoção
 * @param value Valor a somar
 * @return TRANSACTION atualizada
 */

TRANSACTION addQuantity(TRANSACTION t, int month, int MODE, int value) {
	t->quantity[month][MODE] += value;

	return t;	
}

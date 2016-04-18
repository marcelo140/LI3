#ifndef __SALES__
#define __SALES__

#include "generic.h"
#include "clients.h"
#include "products.h"

typedef struct sale    *SALE;
typedef struct revenue *REVENUE;

#define MODE_N 0
#define MODE_P 1

/**
 * Aloca memória para uma SALE. Esta estrutura de dados descreve todos os elementos
 * envolvidos numa transação.
 */
SALE initSale ();

/**
 * Dado uma string correspondente a uma venda, extrai toda a sua informação para uma SALE.
 * @param s SALE que receberá os dados lidos
 * @param p Estrutura para onde serão extraidos os dados sobre o produto vendido
 * @param c Estrutura para onde serão extraidos os dados sobre o cliente que realizaou
 * a transação
 * @return SALE com os dados lidos
 */
SALE readSale (SALE s, PRODUCT p, CLIENT c, char *line);

/**
 * Verifica se todos os dados e uma transação são válidos.
 */
bool isSale (SALE sale, PRODUCTCAT prodCat, CLIENTCAT clientCat);

/**
 * Devolve o produto vendido na transação dada.
 */
PRODUCT getProduct (SALE s);

/**
 * Devolve o cliente que participou na transação dada.
 */
CLIENT getClient (SALE s);

/**
 * Devolve o preço unitário a que foi vendido o produto na transação dada.
 */
double getPrice (SALE s);

/**
 * Devolve a quantidade de produto vendida na transação dada.
 */
int getQuant (SALE s);

/**
 * Devolve o mês em que a transação dada se realizou.
 */
int getMonth (SALE s);

/**
 * Devolve a filial em que a transação dada ocorreu.
 */
int getBranch (SALE s);

/**
 * Devolve o modo em que a transação dada ocorreu, isto é, se o produto se encontrava
 * em modo promoção ou modo normal.
 */
int getMode (SALE s);

/**
 * Liberta todo o espaço reservado para descrever uma venda.
 */
void freeSale (SALE s);

#endif

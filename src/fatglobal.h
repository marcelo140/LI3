#ifndef __FATGLOBAL__
#define __FATGLOBAL__

#include "generic.h"
#include "sales.h"
#include "products.h"
#include "set.h"

typedef struct faturacao *FATGLOBAL;
typedef struct product_fat *PRODUCTFAT;

#define MONTHS 12
#define SALEMODE 2

/**
 * Inicia a faturação global com o número de filiais dado
 */
FATGLOBAL initFat (int branches);

/**
 * Adiciona à faturação global todos os produtos existentes no catálogo de produtos dado.
 */
FATGLOBAL fillFat (FATGLOBAL fat, PRODUCTCAT p);

/**
 * Adiciona os dados de uma venda à faturação.
 */
FATGLOBAL addSaleToFat (FATGLOBAL fat, SALE s);

/**
 * Determina os dados existentes sobre a faturação de um produto num dado mês.
 */
PRODUCTFAT getProductDataByMonth(FATGLOBAL fat, PRODUCT p, int month);

/**
 * Determina o número de vendas efetuadas numa dada filial.
 * @param pf Informação sobre a faturação de um produto num dado mês
 * @param branch Filial pretendida
 * @param normal Vendas ocorridas em modo normal
 * @param promo Vendas ocorridas em modo promoção
 * @return Total de vendas ocorridas
 */
int getProductFatSales(PRODUCTFAT pf, int branch, int* normal, int* promo);

/**
 * Determina a faturação efetuada numa dada filial.
 * @param pf Informação sobre a faturação de um produto num dado mês
 * @param branch Filial pretendida
 * @param normal Faturação efetuada em modo normal
 * @param promo Faturação efetuada em modo promoção
 * @return Faturação total
 */
double getProductFatBilled(PRODUCTFAT pf, int branch, double* normal, double* promo);

/**
 * Calcula a faturação efetuada num dado intervalo de meses.
 */
double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

/**
 * Calcula o número de vendas que ocorreram num dado intervalo de meses.
 */
int getSalesByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

/**
 * Calcula todos os produtos que nunca foram vendidos durante o ano.
 */
SET getProductsNotSold(FATGLOBAL fat);

/**
 * Calcula todos os produtos que nunca foram vendidos em cada uma das filiais.
 * @return Retorna um set de produtos para cada uma das filiais.
 */
SET* getProductsNotSoldByBranch(FATGLOBAL);

/**
 *	Liberta toda a memória associada à faturação global.
 */
void freeFat(FATGLOBAL fat);

/**
 * Liberta memória associado à cópia do conjuto de informações de um produto.
 */
void freeProductFat(PRODUCTFAT pf);

#endif

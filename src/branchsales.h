#ifndef __BRANCHSALES__
#define __BRANCHSALES__

#include "products.h"
#include "clients.h"
#include "sales.h"

typedef struct branchsales *BRANCHSALES;
typedef struct product_data *PRODUCTDATA;

/**
 * Inicia uma filial. Esta estrutura relaciona os clientes de uma filial com
 * as compras efetuadas na mesma.
 */
BRANCHSALES initBranchSales();

/**
 * Adiciona a uma filial todos os clientes e produtos presentes nos catálogos dados.
 */
BRANCHSALES fillBranchSales(BRANCHSALES bs, CLIENTCAT cc, PRODUCTCAT pc);

/**
 * Adiciona os dados da compra ao registo do cliente.
 * @param bs Filial em que foi realizada a compra.
 * @param s Dados acerca da compra efetuada
 */
BRANCHSALES addSaleToBranch(BRANCHSALES bs, SALE s);

/**
 * Determina a quantidade de produtos comprados por um cliente ao longo do ano na
 * filial indicada.
 * @param bs Filial a ser analizada
 * @param c Cliente pedido
 * @return Array de tamanho doze com a quantidade comprada em cada mês
 */
int* getClientQuantByMonth(BRANCHSALES bs, CLIENT c);

/**
 * Determina os clientes que realizaram compras na filial dada.
 * @param bs Filial a ser analizada
 * @return Lista dos clientes que realizaram compras na filial
 */
SET getClientsWhoBought(BRANCHSALES bs);

/**
 * Determina os clientes que nunca compraram na filial dada.
 * @param bs Filial a ser analizada
 * @return Lista dos clientes que nunca compraram
 */
SET getClientsWhoHaveNotBought(BRANCHSALES bs);

/**
 * Determina os clientes de uma dada filial que compraram um dado produto, distinguindo
 * se esta compra foi efetuada quando o produto se encontrava em promoção ou não.
 * @param bs Filial a ser analizada
 * @param prod Produto a ser verificado
 * @param n Lista onde serão armazenados os clientes que compraram em modo normal
 * @param p Lista onde serão armazenados os clientes que compraram em modo promoção
 */
void getClientsByProduct(BRANCHSALES bs, PRODUCT prod, SET *normal, SET *clients);

/**
 * Determina lista de produtos comprados por um dado cliente na filial indicada.
 * @param bs Filial cujos clientes serão analizados
 * @param c Cliente a ser analizado
 * @return Lista de produtos comprados
 */
SET getProductsByClient(BRANCHSALES bs, CLIENT c);

/**
 * Calcula uma lista de produtos ordenados por quantidade. O conteúdo dos elementos da 
 * lista são do tipo PRODUCTDATA podendo ser acedidos para obter informação acerca do
 * produto.
 */
SET listProductsByQuant(BRANCHSALES bs);

/**
 * Calcula o número de clientes que compraram um produto.
 */
int getClientsFromData(PRODUCTDATA pd);

/**
 * Calcula o número de unidades compradas de um produto.
 */
int getQuantFromData(PRODUCTDATA pd);

/**
 * Ordena uma lista de produtos pela quantidade comprada no mês indicado.
 */
void sortProductListByQuant(SET productList, int month);

/**
 * Ordena uma lista de produtos por faturação.
 */
void sortProductListByBilled(SET productList);

/**
 * Liberta toda a memória associada à cópia do conjunto de informações de um produto.
 */
void freeProductData(PRODUCTDATA pd);

/**
 * Liberta toda a memória usada por uma filial.
 */
void freeBranchSales(BRANCHSALES bs);

#endif

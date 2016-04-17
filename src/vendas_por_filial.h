#ifndef __VENDAS_FILIAL__
#define __VENDAS_FILIAL__

#include "sales.h"
#include "generic.h"

typedef struct branch       *BRANCHSALES;
typedef struct client_list  *CLIENTLIST;
typedef struct product_list *PRODUCTLIST;

#define BY_QUANT    0
#define BY_EXPENSES 1
#define BY_NAME     2

/**
 * Inicia uma filial com os clientes dados. Esta esturutra relaciona os clientes de
 * uma filial com as compras efetuadas na mesma.
 */
BRANCHSALES initBranchSales (CLIENTCAT client);

/**
 * Adiciona os dados da compra ao registo do cliente.
 * @param bs Filial em que foi realizada a compra
 * @param s Dados acerca da compra efetuada
 * @return Apontadar para a filial onde foram adicionadas as informações
 */
BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s);

/**
 * Determina os clientes que realizaram compras na filial dada.
 * @param bs Filial a ser analizada
 * @return Lista dos clientes que realizaram compras na filial
 */
CLIENTLIST getClientsWhoBought (BRANCHSALES bs);

/**
 * Determina os clientes que nunca compraram na filial dada.
 * @param bs Filial a ser analizada
 * @return Lista dos clientes que nunca compraram
 */
CLIENTLIST getClientsWhoHaveNotBought (BRANCHSALES bs);

/**
 * Determina os clientes de uma dada filial que compraram um dado produto, distinguindo
 * se esta compra foi efetuada quando o produto se encontrava em promoção ou não.
 * @param bs Filial a ser analizada
 * @param prod Produto a ser verificado
 * @param n Lista onde serão armazenados os clientes que compraram em modo normal
 * @param p Lista onde serão armazenados os clientes que compraram em modo promoção
 */
void filterClientsByProduct (BRANCHSALES bs, PRODUCT prod, CLIENTLIST n, CLIENTLIST p);

/**
 * Determina lista de produtos comprados por um dado cliente na filial indicada.
 * @param bs Filial cujos clientes serão analizados
 * @param c Cliente a ser analizado
 * @return Lista de produtos comprados
 */
PRODUCTLIST filterProductsByClient (BRANCHSALES bs, CLIENT c);

/**
 * Determina a quantidade de produtos comprados por um cliente ao longo do ano na
 * filial indicada.
 * @param bs Filial a ser analizada
 * @param c Cliente pedido
 * @return Array de tamanho doze com a quantidade comprada em cada mês
 */
int* getClientQuant (BRANCHSALES bs, CLIENT c);

/**
 * Determina os gastos de um cliente ao longo do ano na filial indicada.
 * @param bs Filial a ser analizada
 * @param c Clientes pedido
 * @return Array de tamanho doze com os gastos de cada mês
 */
double* getClientExpenses (BRANCHSALES bs, CLIENT c);

/**
 * Liberta toda a memória usada por uma filial.
 */
void freeBranchSale (BRANCHSALES bs);

/**
 * Liberta toda a memória usada por uma lista de clientes.
 */
void freeClientList (CLIENTLIST cl);

/**
 * Liberta toda a memória usada por uma lista de produtos.
 */
void freeProductList (PRODUCTLIST pl);

/**
 * Cria uma lista de clientes.
 */
CLIENTLIST newClientList();

/**
 * Devolve o tamanho da lista de clientes.
 */
int clientListSize(CLIENTLIST cl);

/**
 * Devolve o cliente na posição pos da lista de clientes
 * @param cl Lista de clientes
 * @param pos Posição a devolver;
 * @return o cliente nessa posição
 */
char* getClientListPos(CLIENTLIST cl, int pos);

/**
 * Cria uma lista de produtos.
 */
PRODUCTLIST newProductList();

/**
 * Ordena uma lista de clientes pelo parâmetro pedido.
 * @param cl Lista de clientes a ser ordenada
 * @param mode Modo de ordenação (BY_QUANT, BY_EXPENSES, BY_NAME)
 * @return Lista ordenada pelo modo pedido
 */
CLIENTLIST sortClientList (CLIENTLIST cl, int mode);


/**
 * Ordena uma lista de produtos pelo parâmetro pedido.
 * @param pl Lista de produtos a ser ordenada
 * @param mode Modo de ordenação (BY_QUANT, BY_EXPENSES, BY_NAME)
 * @return Lista ordenada pelo modo pedido
 */
PRODUCTLIST sortProductList (PRODUCTLIST pl, int mode);

#endif

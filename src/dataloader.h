#ifndef __DATALOADER__
#define __DATALOADER__

#include <stdio.h>

#include "generic.h"
#include "branchsales.h"
#include "fatglobal.h"
#include "clients.h"
#include "products.h"

/**
 * Carrega o catálogo de produtos a partir do ficheiro dado.
 * @param file Ficheiro com os produtos a ser lidos
 * @param cat Catálogo a ser preenchido
 * @return Número de produtos corretamente lidos
 */
int loadProducts (FILE *file, PRODUCTCAT cat);

/**
 * Carrega o catálogo de clientes a partir do ficheiro dado.
 * @param file Ficheiro com os clientes a ser lidos
 * @param cat Catálogo a ser preenchido
 * @return Número de clientes corretamente lidos
 */
int loadClients (FILE *file, CLIENTCAT cat);

/**
 * Carrega a Faturação Global e as Filiais com as vendas lidas a partir do ficheiro
 * @param file Ficheiro com as vendas a ser lidas
 * @param fat Módulo de faturação a ser caregado
 * @param bs Módulo de filiais a ser carregado
 * @param products Catálogo com os produtos necessários para validar as vendas
 * @param clients Catálogo com os clientes necessários para validar as vendas
 * @param failed Número de vendas que não foram lidas corretamente
 * @return Número de vendas lidas corretamente
 */
int loadSales (FILE *file, FATGLOBAL fat, BRANCHSALES *bs, PRODUCTCAT products, 
               CLIENTCAT clients, int *failed);

#endif

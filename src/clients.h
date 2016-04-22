#ifndef __CLIENTS__
#define __CLIENTS__

#include "catalog.h"
#include "generic.h"
#include "set.h"

typedef struct client         *CLIENT;
typedef struct client_catalog *CLIENTCAT;

/**
 * Inicializa o catálogo de clientes. Esta estrutura lista todos os clientes existentes
 * indexados pela sua primeira letra.
 */
CLIENTCAT initClientCat ();

/**
 * Cria uma entrada no catálogo de clientes para o cliente indicado.
 */
CLIENTCAT insertClient (CLIENTCAT catalog, CLIENT client);

/**
 * Liberta todo o espaço ocupado por um catálogo de clientes.
 */
void freeClientCat (CLIENTCAT catalog);

/**
 * Verifica se o cliente dado existe no catálogo de clientes.
 */
bool lookUpClient (CLIENTCAT catalog, CLIENT client);

/**
 * Calcula o número de clientes começados por uma dada letra existentes no catálogo.
 */
int countClientes (CLIENTCAT clientCat, char index);

/**
 * Cria uma cópia do catálogo de clientes.
 */
CATALOG getClientCat (CLIENTCAT catalog);

/**
 * Cria um cliente com código nulo.
 */
CLIENT newClient ();

/**
 * Altera o código do cliente dado para a string indicada.
 */
CLIENT changeClientCode(CLIENT c, char* str);

/**
 * Cria um novo cliente com um código corresponde ao da string dada.
 */
CLIENT toClient (char* str);

/**
 * Faz uma cópia do cliente dado.
 */
CLIENT cloneClient (CLIENT c);

/**
 * Devolve uma cópia do código do cliente.
 */
char* fromClient (CLIENT c);

/**
 * Verifica se cliente contém algum código associado.
 */
bool isEmptyClient (CLIENT c);

/**
 * Liberta todo o espaço ocupado por um cliente.
 */
void freeClient (CLIENT c);

/**
 * Preenche o conjunto indicado com todos os clientes começados pela letra dada 
 * existentes no catálogo de clientes.
 */
SET fillClientSet (CLIENTCAT clientCat, char index);

#endif

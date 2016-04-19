#ifndef __CLIENTS__
#define __CLIENTS__

#include "catalog.h"
#include "generic.h"
#include "set.h"

#define CLIENT_LENGTH 6

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
CLIENT writeClient (CLIENT c, char* str);

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
 * @param c Clietne pretendido.
 * @param dest Buffer onde será guardada a cópia do código do cliente
 */
char* fromClient (CLIENT c, char* dest);

/**
 * Verifica se cliente contém algum código associado.
 */
bool isEmptyClient (CLIENT c);

/**
 * Verifica se a string dada corresponde a um código de cliente válido.
 */
bool isClient (char* str);

/**
 * Liberta todo o espaço ocupado por um cliente.
 */
void freeClient (CLIENT c);


/**
 * Preenche o conjunto indicado com todos os clientes começados pela letra dada 
 * existentes no catálogo de clientes. Se o conjunto não tiver espaço suficiente para
 * acatar todos os clientes, este será redimensionado para o tamnha necessário.
 */
LIST fillClientSet (CLIENTCAT clientCat, char index);
#endif

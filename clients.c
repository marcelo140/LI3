#include <stdlib.h>
#include <string.h>
#include "clients.h"

#define MAX_BUFF 10

struct client{
	char *str;
};

/** Inicia o Catálogo de Clientes
 * @param void
 * @return Catálogo de Clientes novo
 */
CLIENTCAT initClientCat() {
    return initCatalog();
}

/** Insere cliente no catálogo de Clientes.
 * @param CLIENTCAT Catálogo de Clientes
 * @param CLIENT Cliente a inserir
 * @return Catálogo de Clientes com o novo cliente inserido.
 */
CLIENTCAT insertClient(CLIENTCAT catalog, CLIENT client) {
	return insertCatalog(catalog, client.str);
}

/** Verifica se um dado cliente existe num dado catálogo
 * @param CLIENTCATALOG Catálogo de Clientes
 * @param CLIENT Cliente a procurar
 * @return 1 se encontrou, 0 caso contrário
 */
bool lookUpClient(CLIENTCAT catalog, CLIENT client) {
	return lookUpCatalog(catalog, client.str);
}

/** Liberta o espaço ocupado pelo Catálogo de Clientes
 * @param CLIENTCAT Catálogo de Clientes
 * @return void
 */
void freeClientCat(CLIENTCAT catalog) {
	freeCatalog(catalog);
}

/** Converte String para CLIENT
 *  @param char* String a converter
 *  @return CLIENT
 */
CLIENT toClient(char *str) {

	CLIENT r;
	r.str = malloc (MAX_BUFF);
	strncpy(r.str, str, MAX_BUFF);

	return r;
}

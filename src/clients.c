#include <stdlib.h>
#include <string.h>

#include "catalog.h"
#include "clients.h"

#define MAX_BUFF 10
#define CLIENT_LENGTH 6

struct clientCat {
		CATALOG cat;
};

struct client{
	char *str;
};

/** 
 * Inicia o Catálogo de Clientes
 * @return Catálogo de Clientes novo
 */
CLIENTCAT initClientCat() {
	CLIENTCAT cCat = malloc(sizeof (struct clientCat));
	cCat->cat = initCatalog();

    return cCat;
}

/** 
 * Insere cliente no catálogo de Clientes.
 * @param cCat Catálogo de Clientes
 * @param client Cliente a inserir
 * @return Catálogo de Clientes com o novo cliente inserido.
 */
CLIENTCAT insertClient(CLIENTCAT cCat, CLIENT client) {
	cCat->cat = insertCatalog(cCat->cat, client->str, NULL);

	return cCat;
}

/** 
 * Verifica se um dado cliente existe num dado catálogo
 * @param cCat Catálogo de Clientes
 * @param client Cliente a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpClient(CLIENTCAT cCat, CLIENT client) {
	return lookUpCatalog(cCat->cat, client->str);
}

/** 
 * Liberta o espaço ocupado pelo Catálogo de Clientes
 * @param cCat Catálogo de Clientes
 */
void freeClientCat(CLIENTCAT cCat) {
	freeCatalog(cCat->cat);
	free(cCat);
}

/** 
 * Converte String para CLIENT
 * @param str String a converter
 * @return CLIENT novo
 */
CLIENT toClient(char *str) {
	CLIENT r = malloc(sizeof (*r));

/*	if (!isClient(str))
		return NULL;
*/
	r->str = malloc (MAX_BUFF);
	strncpy(r->str, str, MAX_BUFF);

	return r;
}

char *fromClient(CLIENT c) {
	return c->str;
}

CLIENT cloneClient(CLIENT c) {
	
	CLIENT new = malloc(sizeof(*new));
	new->str = malloc(MAX_BUFF);

	strcpy(new->str, c->str);

	return new;	
}

bool isClient(char *str) {
	int i;
	bool client = true;

	for (i = 0; client && i < CLIENT_LENGTH; i++){
		switch (i){
			case 0: client = (str[i] >= 'A' && str[i] <= 'Z');
					break;
			case 1: client = (str[i] >= '1' && str[i] <= '5');
					break;
			case 2:
			case 3:
			case 4: client = (str[1] == '5' && str[i] == '0') ||
							   (str[1] != '5' && str[i] >= '0' && str[i] <= '9');
					break;
		}
	}

	return client;
}

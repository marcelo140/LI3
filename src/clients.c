#include <stdlib.h>
#include <string.h>

#include "catalog.h"
#include "clients.h"

#define INDEX(c)             (c->str[0] - 'A')

#define IS_CAPITAL_LETTER(c) (c >= 'A' && c <= 'Z')
#define IS_IN_RANGE(c, i, j) (c >= i   && c <= j)
#define IS_NUMBER(c)         (c >= '0' && c <= '9')

struct client{
	char str[CLIENT_LENGTH];
};

struct client_catalog {
		CATALOG cat;
};

struct client_set {
	CATSET set;
};

/** 
 * Inicia o Catálogo de Clientes
 * @return Catálogo de Clientes novo
 */
CLIENTCAT initClientCat() {
	CLIENTCAT clientCat = malloc(sizeof (*clientCat));
	clientCat->cat = initCatalog(26, NULL, NULL, NULL, NULL, NULL);

    return clientCat;
}

/** 
 * Insere cliente no catálogo de Clientes.
 * @param clientCat Catálogo de Clientes
 * @param client Cliente a inserir
 * @return Catálogo de Clientes com o novo cliente inserido.
 */
CLIENTCAT insertClient(CLIENTCAT clientCat, CLIENT client) {
	clientCat->cat = insertCatalog(clientCat->cat, INDEX(client), client->str, NULL);

	return clientCat;
}

/** 
 * Liberta o espaço ocupado pelo Catálogo de Clientes
 * @param clientCat Catálogo de Clientes
 */
void freeClientCat(CLIENTCAT clientCat) {
	freeCatalog(clientCat->cat);
	free(clientCat);
}

/** 
 * Verifica se um dado cliente existe num dado catálogo
 * @param clientCat Catálogo de Clientes
 * @param client Cliente a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpClient(CLIENTCAT clientCat, CLIENT client) {
	return lookUpCatalog(clientCat->cat, INDEX(client), client->str);
}

/**
 * @return Retorna cliente não inicializado
 */
CLIENT newClient() {
	return malloc(sizeof(struct client));
}

/** 
 * Converte String para CLIENT
 * @param str String a converter
 * @return CLIENT novo
 */
CLIENT toClient(char* str) {
	CLIENT r = malloc(sizeof (*r));

/*	if (!isClient(str))
		return NULL;
*/

	strncpy(r->str, str, CLIENT_LENGTH);

	return r;
}

/**
 * Atribui a string dada ao cliente
 * @param c Cliente a ser alteado
 * @param str String dada
 * @return Cliente alterado
 */
CLIENT writeClient(CLIENT c, char* str) {
	strncpy(c->str, str, CLIENT_LENGTH);
	return c;
}

CLIENT cloneClient(CLIENT c) {
	CLIENT new;

	new = malloc(sizeof(*new));
	strcpy(new->str, c->str);

	return new;	
}

char* fromClient(CLIENT c, char* dest) {
	strncpy(dest, c->str, CLIENT_LENGTH);
	return dest;
}

/**
 * Liberta memória ocupada pelo cliente
 * @param c Cliente a ser libertado
 */
void freeClient(CLIENT c) {
	free(c);
}

bool isClient(char *str) {
	return IS_CAPITAL_LETTER(str[0]) &&
           /* Se str[1] é '5', todos os carateres seguintes são '0' */
	       ((str[1] == '5' && str[2] == '0' && str[3] == '0' && str[4] == '0') ||
           /* Caso contrário, todos os carateres seguintes devem estar entre '0' e '9' */
		    (IS_IN_RANGE(str[1], '1', '4') && IS_IN_RANGE(str[2], '0', '9')
                                           && IS_IN_RANGE(str[3], '0', '9')
                                           && IS_IN_RANGE(str[4], '0', '9')));
}

/**
 * Inicializa um CLIENTSET com um tamanho n
 * @param n Tamanho mínimo do CLIENTSET
 * @return CLIENTSET inicializado
 */
CLIENTSET initClientSet(int n) {
	CLIENTSET new = malloc (sizeof (*new));
	new->set = initCatalogSet(n);

	return new;
}

/**
 * @param catProd Catálogo onde se encontra a informação pretendida
 * @param cs Set onde se pretende guardar a informação
 * @param index Posição do catálogo onde se encontra a informação
 * @return Set preenchido
 */
CLIENTSET fillClientSet(CLIENTCAT catProd, CLIENTSET cs, char index) {
	cs->set = fillCatalogSet(catProd->cat, cs->set, index - 'A');

	return cs;
}

void freeClientSet(CLIENTSET cs) {
	freeCatalogSet(cs->set);
	free(cs);	
}

/**
 * @param cs Set de clientes
 * @param pos Posição do cliente
 * @return Cliente
 */
CLIENT getClientByPos(CLIENTSET cs, int pos) {
	char *str = getKeyPos(cs->set, pos);
	
	return toClient(str);
}

int getClientSetSize(CLIENTSET cs) {
	return getCatalogSetSize(cs->set);
}


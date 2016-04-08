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

struct clientSet {
	DATASET set;
};

/** 
 * Inicia o Catálogo de Clientes
 * @return Catálogo de Clientes novo
 */
CLIENTCAT initClientCat() {
	CLIENTCAT cCat = malloc(sizeof (struct clientCat));
	cCat->cat = initCatalog(26);

    return cCat;
}

/** 
 * Insere cliente no catálogo de Clientes.
 * @param cCat Catálogo de Clientes
 * @param client Cliente a inserir
 * @return Catálogo de Clientes com o novo cliente inserido.
 */
CLIENTCAT insertClient(CLIENTCAT cCat, CLIENT client) {
	cCat->cat = insertCatalog(cCat->cat, client->str[0] - 'A', client->str, NULL);

	return cCat;
}

/** 
 * Verifica se um dado cliente existe num dado catálogo
 * @param cCat Catálogo de Clientes
 * @param client Cliente a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpClient(CLIENTCAT cCat, CLIENT client) {
	return lookUpCatalog(cCat->cat, client->str[0] - 'A', client->str);
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

/**
 * Liberta memória ocupada pelo cliente
 * @param c Cliente a ser libertado
 */
void freeClient(CLIENT c) {
	free(c->str);
	free(c);
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


/**
 * Inicializa um CLIENTSET com um tamanho n
 * @param n Tamanho mínimo do CLIENTSET
 * @return CLIENTSET inicializado
 */
CLIENTSET initCset(int n) {
	CLIENTSET new = malloc (sizeof (*new));
	new->set = initDataSet(n);

	return new;
}

/**
 * @param catProd Catálogo onde se encontra a informação pretendida
 * @param cs Set onde se pretende guardar a informação
 * @param index Posição do catálogo onde se encontra a informação
 * @return Set preenchido
 */
CLIENTSET fillCset(CLIENTCAT catProd, CLIENTSET cs, char index) {
	cs->set = fillDataSet(catProd->cat, cs->set, index - 'A');

	return cs;
}

/**
 * @param cs Set de clientes
 * @param pos Posição do cliente
 * @return Cliente
 */
CLIENT getCsetData(CLIENTSET cs, int pos) {
	char *str = getDataSet(cs->set, pos);
	
	return toClient(str);
}

int csetSize(CLIENTSET cs) {
	return getDataSetSize(cs->set);
}

void freeCSet(CLIENTSET cs) {
	freeDataSet(cs->set);
	free(cs);	
}

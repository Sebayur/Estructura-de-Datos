#ifndef ABB_ESTRUCTURA_PRIVADA_H_
#define ABB_ESTRUCTURA_PRIVADA_H_

#include <stdlib.h>

//ESTE ARCHIVO NO ES PARA EL USUARIO
//
//NO TE QUIERO VER INCLUYENDO ESTO EN TU PROGRAMA

typedef struct nodo_t {
	void *elemento;
	struct nodo_t *izq;
	struct nodo_t *der;
} nodo_t;

struct abb_t {
	size_t nodos;
	nodo_t *raiz;
	int (*comparador)(const void *, const void *);
};

#endif // ABB_ESTRUCTURA_PRIVADA_H_

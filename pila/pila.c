#include "pila.h"
#include "lista.h"
#include <stdbool.h>
#include <stdio.h>

struct pila {
	lista_t *lista;
};

pila_t *pila_crear()
{
	pila_t *pila = malloc(sizeof(pila_t));
	if (!pila)
		return NULL;
	pila->lista = lista_crear();
	if (!pila->lista) {
		free(pila);
		return NULL;
	}
	return pila;
}

bool pila_apilar(pila_t *pila, void *elemento)
{
	if (!pila || !elemento)
		return false;
	return lista_insertar(pila->lista, elemento);
}

void *pila_desapilar(pila_t *pila)
{
	if (!pila || pila_vacia(pila))
		return NULL;
	return lista_sacar_de_posicion(pila->lista,
				       (int)(lista_tamanio(pila->lista) - 1));
}

bool pila_vacia(pila_t *pila)
{
	if (!pila)
		return true;
	if (lista_tamanio(pila->lista) == 0)
		return true;
	return false;
}

size_t pila_tamanio(pila_t *pila)
{
	if (!pila)
		return 0;
	return lista_tamanio(pila->lista);
}

void *pila_tope(pila_t *pila)
{
	if (!pila || lista_tamanio(pila->lista) == 0)
		return NULL;
	return pila->lista->ultimo->elemento;
}

void pila_destruir(pila_t *pila)
{
	if (!pila)
		return;
	lista_destruir(pila->lista);
	free(pila);
}
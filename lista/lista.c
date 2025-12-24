#include "lista.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *primero;
	nodo_t *ultimo;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *actual;
};

lista_t *lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));
	if (!lista)
		return NULL;
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->cantidad = 0;
	return lista;
}

bool lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return false;
	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;
	nuevo->elemento = elemento;
	nuevo->siguiente = NULL;
	if (!lista->primero) {
		lista->primero = nuevo;
	} else {
		lista->ultimo->siguiente = nuevo;
	}
	lista->ultimo = nuevo;
	lista->cantidad++;
	return true;
}

bool lista_insertar_en_posicion(lista_t *lista, int posicion, void *elemento)
{
	if (!lista || posicion < 0 || posicion > lista->cantidad)
		return false;
	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;
	nuevo->elemento = elemento;
	if (posicion == 0) {
		nuevo->siguiente = lista->primero;
		lista->primero = nuevo;
		if (!lista->ultimo)
			lista->ultimo = nuevo;
	} else {
		nodo_t *actual = lista->primero;
		for (int i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}
		nuevo->siguiente = actual->siguiente;
		actual->siguiente = nuevo;
		if (!nuevo->siguiente)
			lista->ultimo = nuevo;
	}
	lista->cantidad++;
	return true;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;
	return lista->cantidad;
}

void *lista_obtener_elemento(lista_t *lista, int posicion)
{
	if (!lista || posicion < 0 || posicion >= lista->cantidad)
		return NULL;
	nodo_t *actual = lista->primero;
	for (int i = 0; i < posicion; i++) {
		actual = actual->siguiente;
	}
	return actual->elemento;
}

void *lista_sacar_de_posicion(lista_t *lista, int posicion)
{
	if (!lista || posicion < 0 || posicion >= lista->cantidad)
		return NULL;
	nodo_t *a_eliminar;
	void *elemento;
	if (posicion == 0) {
		a_eliminar = lista->primero;
		lista->primero = lista->primero->siguiente;
		if (lista->cantidad == 1)
			lista->ultimo = NULL;
	} else {
		nodo_t *actual = lista->primero;
		for (int i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}
		a_eliminar = actual->siguiente;
		actual->siguiente = a_eliminar->siguiente;
		if (!a_eliminar->siguiente)
			lista->ultimo = actual;
	}
	elemento = a_eliminar->elemento;
	free(a_eliminar);
	lista->cantidad--;
	return elemento;
}

void *lista_sacar_elemento(lista_t *lista, void *elemento)
{
	if (!lista)
		return NULL;
	nodo_t *actual = lista->primero, *anterior = NULL;
	while (actual) {
		if (actual->elemento == elemento) {
			if (anterior) {
				anterior->siguiente = actual->siguiente;
				if (!actual->siguiente)
					lista->ultimo = anterior;
			} else {
				lista->primero = actual->siguiente;
				if (lista->cantidad == 1)
					lista->ultimo = NULL;
			}
			void *elemento_sacado = actual->elemento;
			free(actual);
			lista->cantidad--;
			return elemento_sacado;
		}
		anterior = actual;
		actual = actual->siguiente;
	}
	return NULL;
}

int lista_buscar_posicion(lista_t *lista, void *elemento)
{
	if (!lista)
		return -1;
	nodo_t *actual = lista->primero;
	int posicion = 0;
	while (actual) {
		if (actual->elemento == elemento)
			return posicion;
		actual = actual->siguiente;
		posicion++;
	}
	return -1;
}

void *lista_buscar(lista_t *lista, bool (*criterio)(void *, void *),
		   void *contexto)
{
	if (!lista || !criterio)
		return false;
	nodo_t *actual = lista->primero;
	while (actual) {
		if (criterio(actual->elemento, contexto))
			return actual->elemento;
		actual = actual->siguiente;
	}
	return NULL;
}

int lista_iterar(lista_t *lista, bool (*f)(void *, void *), void *contexto)
{
	if (!lista || !f)
		return 0;
	nodo_t *actual = lista->primero;
	int contador = 0;
	while (actual) {
		contador++;
		if (!f(actual->elemento, contexto))
			break;
		actual = actual->siguiente;
	}
	return contador;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;
	nodo_t *actual = lista->primero;
	while (actual) {
		nodo_t *a_liberar = actual;
		actual = actual->siguiente;
		free(a_liberar);
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;
	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;
	iterador->actual = lista->primero;
	return iterador;
}

bool lista_iterador_quedan_elementos_por_recorrer(lista_iterador_t *iterador)
{
	if (!iterador)
		return false;
	return iterador->actual != NULL;
}

void lista_iterador_proxima_iteracion(lista_iterador_t *iterador)
{
	if (!iterador)
		return;
	if (iterador->actual) {
		iterador->actual = iterador->actual->siguiente;
	}
}

void *lista_iterador_obtener_elemento(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->actual)
		return NULL;
	return iterador->actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (!iterador)
		return;
	free(iterador);
}
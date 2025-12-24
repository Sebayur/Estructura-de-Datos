#include "cola.h"
#include "lista.h"
#include <stdbool.h>
#include <stdio.h>

struct cola {
	lista_t *lista;
};

cola_t *cola_crear()
{
	cola_t *cola = malloc(sizeof(cola_t));
	if (!cola)
		return NULL;
	cola->lista = lista_crear();
	if (!cola->lista) {
		free(cola);
		return NULL;
	}
	return cola;
}

bool cola_encolar(cola_t *cola, void *elemento)
{
	if (!cola)
		return false;
	return lista_insertar(cola->lista, elemento);
}

void *cola_desencolar(cola_t *cola)
{
	if (!cola || lista_tamanio(cola->lista) == 0)
		return NULL;
	return lista_sacar_de_posicion(cola->lista, 0);
}

bool cola_vacia(cola_t *cola)
{
	if (!cola || lista_tamanio(cola->lista) == 0)
		return true;
	return false;
}

size_t cola_tamanio(cola_t *cola)
{
	if (!cola)
		return 0;
	return lista_tamanio(cola->lista);
}

void *cola_frente(cola_t *cola)
{
	if (!cola || lista_tamanio(cola->lista) == 0)
		return NULL;
	return lista_obtener_elemento(cola->lista, 0);
}

void cola_destruir(cola_t *cola)
{
	if (!cola)
		return;
	lista_destruir(cola->lista);
	free(cola);
}
#include "abb.h"
#include "abb_estructura_privada.h"
#include <stdio.h>

static nodo_t *nodo_crear(const void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo) {
		return NULL;
	}
	nodo->elemento = (void *)elemento;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

static void nodo_destruir(nodo_t *nodo)
{
	if (!nodo)
		return;
	free(nodo);
}

static bool recorrer_recursivo(nodo_t *nodo, enum abb_recorrido modo,
			       bool (*f)(void *, void *), void *ctx,
			       size_t *cont)
{
	if (!nodo)
		return true;
	if (modo == ABB_PREORDEN) {
		(*cont)++;
		if (!f((void *)nodo->elemento, ctx))
			return false;
	}
	if (!recorrer_recursivo(nodo->izq, modo, f, ctx, cont))
		return false;
	if (modo == ABB_INORDEN) {
		(*cont)++;
		if (!f((void *)nodo->elemento, ctx))
			return false;
	}
	if (!recorrer_recursivo(nodo->der, modo, f, ctx, cont))
		return false;
	if (modo == ABB_POSTORDEN) {
		(*cont)++;
		if (!f((void *)nodo->elemento, ctx))
			return false;
	}
	return true;
}

static bool vectorizar_recursivo(nodo_t *nodo, enum abb_recorrido modo,
				 void **vector, size_t *pos, size_t capacidad)
{
	if (!nodo || *pos >= capacidad)
		return *pos < capacidad;
	if (modo == ABB_PREORDEN) {
		if (*pos >= capacidad)
			return false;
		vector[(*pos)++] = (void *)nodo->elemento;
		if (!vectorizar_recursivo(nodo->izq, modo, vector, pos,
					  capacidad))
			return false;
		if (!vectorizar_recursivo(nodo->der, modo, vector, pos,
					  capacidad))
			return false;
	} else if (modo == ABB_INORDEN) {
		if (!vectorizar_recursivo(nodo->izq, modo, vector, pos,
					  capacidad))
			return false;
		if (*pos >= capacidad)
			return false;
		vector[(*pos)++] = (void *)nodo->elemento;
		if (!vectorizar_recursivo(nodo->der, modo, vector, pos,
					  capacidad))
			return false;
	} else if (modo == ABB_POSTORDEN) {
		if (!vectorizar_recursivo(nodo->izq, modo, vector, pos,
					  capacidad))
			return false;
		if (!vectorizar_recursivo(nodo->der, modo, vector, pos,
					  capacidad))
			return false;
		if (*pos >= capacidad)
			return false;
		vector[(*pos)++] = (void *)nodo->elemento;
	}
	return true;
}

static void destruir_solo_nodos(nodo_t *nodo)
{
	if (!nodo)
		return;
	destruir_solo_nodos(nodo->izq);
	destruir_solo_nodos(nodo->der);
	free(nodo);
}

static void destruir_con_elementos(nodo_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;
	destruir_con_elementos(nodo->izq, destructor);
	destruir_con_elementos(nodo->der, destructor);
	if (destructor && nodo->elemento) {
		destructor((void *)nodo->elemento);
	}
	free(nodo);
}

abb_t *abb_crear(int (*cmp)(const void *, const void *))
{
	if (!cmp)
		return NULL;
	abb_t *abb = malloc(sizeof(abb_t));
	if (!abb) {
		return NULL;
	}
	abb->raiz = NULL;
	abb->nodos = 0;
	abb->comparador = cmp;
	return abb;
}

bool abb_insertar(abb_t *abb, const void *elemento)
{
	if (!abb)
		return false;
	nodo_t *nuevo = nodo_crear(elemento);
	if (!nuevo) {
		return false;
	}
	if (!abb->raiz) {
		abb->raiz = nuevo;
		abb->nodos++;
		return true;
	}
	nodo_t *actual = abb->raiz;
	while (true) {
		int cmp = abb->comparador(elemento, actual->elemento);
		if (cmp < 0) {
			if (!actual->izq) {
				actual->izq = nuevo;
				break;
			}
			actual = actual->izq;
		} else {
			if (!actual->der) {
				actual->der = nuevo;
				break;
			}
			actual = actual->der;
		}
	}
	abb->nodos++;
	return true;
}

bool abb_existe(const abb_t *abb, const void *elemento)
{
	if (!abb || !elemento)
		return false;
	nodo_t *actual = abb->raiz;
	while (actual) {
		int cmp = abb->comparador(elemento, actual->elemento);
		if (cmp == 0)
			return true;
		if (cmp < 0) {
			actual = actual->izq;
		} else {
			actual = actual->der;
		}
	}
	return false;
}

void *abb_buscar(const abb_t *abb, const void *elemento)
{
	if (!abb || !elemento)
		return NULL;
	nodo_t *actual = abb->raiz;
	while (actual) {
		int cmp = abb->comparador(elemento, actual->elemento);
		if (cmp == 0)
			return (void *)actual->elemento;
		if (cmp < 0) {
			actual = actual->izq;
		} else {
			actual = actual->der;
		}
	}
	return NULL;
}

void *abb_sacar(abb_t *abb, const void *elemento)
{
	if (!abb || !abb->raiz)
		return NULL;
	nodo_t *actual = abb->raiz;
	nodo_t *padre = NULL;
	nodo_t *sustituto = NULL;
	while (actual) {
		int cmp = abb->comparador(elemento, actual->elemento);
		if (cmp == 0)
			break;
		padre = actual;
		actual = (cmp < 0) ? actual->izq : actual->der;
	}
	if (!actual)
		return NULL;
	void *elemento_eliminado = (void *)actual->elemento;
	if (!actual->izq && !actual->der) {
		if (padre) {
			if (padre->izq == actual)
				padre->izq = NULL;
			else
				padre->der = NULL;
		} else {
			abb->raiz = NULL;
		}
		nodo_destruir(actual);
	} else if (!actual->izq || !actual->der) {
		nodo_t *hijo = actual->izq ? actual->izq : actual->der;
		if (padre) {
			if (padre->izq == actual)
				padre->izq = hijo;
			else
				padre->der = hijo;
		} else {
			abb->raiz = hijo;
		}
		nodo_destruir(actual);
	} else {
		sustituto = actual->izq;
		nodo_t *padre_sustituto = actual;
		while (sustituto->der) {
			padre_sustituto = sustituto;
			sustituto = sustituto->der;
		}
		actual->elemento = sustituto->elemento;
		if (padre_sustituto->der == sustituto)
			padre_sustituto->der = sustituto->izq;
		else
			padre_sustituto->izq = sustituto->izq;
		nodo_destruir(sustituto);
	}
	abb->nodos--;
	return elemento_eliminado;
}

size_t abb_tamanio(const abb_t *abb)
{
	if (!abb)
		return 0;
	return abb->nodos;
}

bool abb_vacio(const abb_t *abb)
{
	if (!abb)
		return true;
	return abb->nodos == 0;
}

size_t abb_recorrer(const abb_t *abb, enum abb_recorrido modo,
		    bool (*f)(void *, void *), void *ctx)
{
	if (!abb || !f)
		return 0;
	size_t cont = 0;
	recorrer_recursivo(abb->raiz, modo, f, ctx, &cont);
	return cont;
}

size_t abb_vectorizar(const abb_t *abb, enum abb_recorrido modo, void **vector,
		      size_t capacidad)
{
	if (!abb || !vector || capacidad == 0)
		return 0;
	size_t pos = 0;
	vectorizar_recursivo(abb->raiz, modo, vector, &pos, capacidad);
	return pos;
}

void abb_destruir(abb_t *abb)
{
	if (!abb)
		return;
	destruir_solo_nodos(abb->raiz);
	free(abb);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;
	destruir_con_elementos(abb->raiz, destructor);
	free(abb);
}
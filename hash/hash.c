#include "hash.h"
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_MINIMA 3
#define FACTOR_CARGA_MAXIMO 0.7

typedef enum { VACIO, OCUPADO, BORRADO } estado_t;

typedef struct {
	char *clave;
	void *valor;
	estado_t estado;
} campo_t;

struct hash {
	campo_t *tabla;
	size_t capacidad;
	size_t cantidad;
	size_t (*funcion_hash)(const char *);
};

static size_t hash_djb2(const char *str)
{
	size_t hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + (size_t)c;
	return hash;
}

static char *duplicar_clave(const char *clave)
{
	char *nueva = malloc(strlen(clave) + 1);
	if (nueva)
		strcpy(nueva, clave);
	return nueva;
}

static size_t buscar_posicion(campo_t *tabla, size_t capacidad,
			      const char *clave, size_t (*f_hash)(const char *),
			      bool *encontrado)
{
	size_t hash = f_hash(clave);
	size_t pos;
	for (size_t i = 0; i < capacidad; i++) {
		pos = (hash + i) % capacidad;

		if (tabla[pos].estado == VACIO) {
			*encontrado = false;
			return pos;
		}

		if (tabla[pos].estado == OCUPADO &&
		    strcmp(tabla[pos].clave, clave) == 0) {
			*encontrado = true;
			return pos;
		}
	}
	*encontrado = false;
	return capacidad;
}

static bool redimensionar(hash_t *hash, size_t nueva_capacidad)
{
	campo_t *nueva_tabla = calloc(nueva_capacidad, sizeof(campo_t));
	if (!nueva_tabla)
		return false;

	for (size_t i = 0; i < hash->capacidad; i++) {
		campo_t campo = hash->tabla[i];
		if (campo.estado == OCUPADO) {
			bool encontrado;
			size_t pos = buscar_posicion(
				nueva_tabla, nueva_capacidad, campo.clave,
				hash->funcion_hash, &encontrado);
			nueva_tabla[pos].clave = campo.clave;
			nueva_tabla[pos].valor = campo.valor;
			nueva_tabla[pos].estado = OCUPADO;
		}
	}

	free(hash->tabla);
	hash->tabla = nueva_tabla;
	hash->capacidad = nueva_capacidad;

	return true;
}

hash_t *hash_crear(size_t capacidad)
{
	return hash_crear_con_funcion(capacidad, hash_djb2);
}

hash_t *hash_crear_con_funcion(size_t capacidad, size_t (*f)(const char *))
{
	if (!f)
		return NULL;
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash)
		return NULL;

	hash->tabla = calloc(capacidad, sizeof(campo_t));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}

	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->funcion_hash = f;

	return hash;
}

bool hash_insertar(hash_t *hash, const char *clave, void *valor,
		   void **anterior)
{
	if (!hash || !clave)
		return false;

	if ((double)(hash->cantidad + 1) / (double)(hash->capacidad) >
	    FACTOR_CARGA_MAXIMO) {
		if (!redimensionar(hash, hash->capacidad * 2))
			return false;
	}

	bool encontrado;
	size_t pos = buscar_posicion(hash->tabla, hash->capacidad, clave,
				     hash->funcion_hash, &encontrado);

	if (encontrado) {
		if (anterior)
			*anterior = hash->tabla[pos].valor;
		hash->tabla[pos].valor = valor;
	} else {
		if (anterior)
			*anterior = NULL;
		char *clave_dup = duplicar_clave(clave);
		if (!clave_dup)
			return false;

		hash->tabla[pos].clave = clave_dup;
		hash->tabla[pos].valor = valor;
		hash->tabla[pos].estado = OCUPADO;
		hash->cantidad++;
	}

	return true;
}

void *hash_sacar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	bool encontrado;
	size_t pos = buscar_posicion(hash->tabla, hash->capacidad, clave,
				     hash->funcion_hash, &encontrado);
	if (!encontrado)
		return NULL;

	void *valor = hash->tabla[pos].valor;
	free(hash->tabla[pos].clave);
	hash->tabla[pos].clave = NULL;
	hash->tabla[pos].valor = NULL;
	hash->tabla[pos].estado = BORRADO;
	hash->cantidad--;

	return valor;
}

void *hash_buscar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	bool encontrado;
	size_t pos = buscar_posicion(hash->tabla, hash->capacidad, clave,
				     hash->funcion_hash, &encontrado);
	return encontrado ? hash->tabla[pos].valor : NULL;
}

bool hash_existe(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	bool encontrado;
	buscar_posicion(hash->tabla, hash->capacidad, clave, hash->funcion_hash,
			&encontrado);
	return encontrado;
}

size_t hash_tamanio(hash_t *hash)
{
	return hash ? hash->cantidad : 0;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].estado == OCUPADO) {
			free(hash->tabla[i].clave);
			if (destructor)
				destructor(hash->tabla[i].valor);
		}
	}

	free(hash->tabla);
	free(hash);
}

size_t hash_iterar_claves(hash_t *hash, bool (*f)(const char *, void *),
			  void *ctx)
{
	if (!hash || !f)
		return 0;

	size_t llamadas = 0;
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].estado == OCUPADO) {
			llamadas++;
			if (!f(hash->tabla[i].clave, ctx))
				break;
		}
	}

	return llamadas;
}

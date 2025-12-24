#ifndef ABB_H_
#define ABB_H_
#include <stdlib.h>
#include <stdbool.h>

enum abb_recorrido { ABB_INORDEN, ABB_PREORDEN, ABB_POSTORDEN };

typedef struct abb_t abb_t;

/**
 * Crea un ABB vacío con el comparador dado.abb_buscar
 * En caso de error devuelve NULL.
 */
abb_t *abb_crear(int (*cmp)(const void *, const void *));

/**
 * Inserta un elemento en el ABB. Admite elementos repetidos.
 * 
 * Devuelve true si se pudo insertar el elemento, false si no pudo.
 */
bool abb_insertar(abb_t *abb, const void *elemento);

/**
 * Busca un elemento en el ABB.
 * 
 * Devuelve true si el elemento existe, false si no existe o en caso de error.
 */
bool abb_existe(const abb_t *abb, const void *elemento);

/**
 * Busca un elemento en el ABB y lo devuelve.
 * 
 * Devuelve el elemento si existe, NULL si no existe o en caso de error.
 */
void *abb_buscar(const abb_t *abb, const void *elemento);

/**
 * Busca un elemento en el ABB y lo elimina.
 * 
 * Devuelve el elemento eliminado si existe, NULL si no existe o en caso de error.
 */
void *abb_sacar(abb_t *abb, const void *elemento);

/**
 * Devuelve la cantidad de elementos en el ABB.
 */
size_t abb_tamanio(const abb_t *abb);

/**
 * Devuelve true si el ABB está vacío, false si no lo está.
 * 
 * Un árbol que no existe también se considera vacío.
 */
bool abb_vacio(const abb_t *abb);

/**
 * Recorre el ABB en el orden indicado y aplica la función f a cada elemento.
 * 
 * Devuelve la cantidad de elementos recorridos.
 */
size_t abb_recorrer(const abb_t *abb, enum abb_recorrido modo,
		    bool (*f)(void *, void *), void *ctx);

/**
 * Recorre el ABB en el orden indicado y guarda los elementos en el vector, sin exceder la capacidad.
 * 
 * Devuelve la cantidad de elementos guardados.
 */
size_t abb_vectorizar(const abb_t *abb, enum abb_recorrido modo, void **vector,
		      size_t capacidad);

/**
 * Destruye el ABB y libera toda la memoria asignada.
 */
void abb_destruir(abb_t *abb);

/**
 * Destruye el ABB y libera toda la memoria asignada, aplicando la función destructor a cada elemento.
 * 
 * Si el destructor es NULL, no se aplica ninguna función a los elementos.
 */
void abb_destruir_todo(abb_t *abb, void (*destructor)(void *));

#endif // ABB_H_

#ifndef LISTA_H_
#define LISTA_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct lista lista_t;
typedef struct lista_iterador lista_iterador_t;

/**
 * Crea una lista vacía.
 *
 */
lista_t *lista_crear();

/**
 * Inserta el elemento al final de la lista.
 *
 * Devuelve true si pudo insertar o false si no pudo
 */
bool lista_insertar(lista_t *lista, void *elemento);

/**
 * Inserta un elemento en la posición dada en la lista.
 *
 * Si la posición excede al tamaño de la lista, no se inserta nada.
 *
 * Devuelve true si pudo insertar o false si no pudo
 */
bool lista_insertar_en_posicion(lista_t *lista, int posicion, void *elemento);

/**
 * Devuelve la cantidad de elementos presentes en la lista.
 *
 * Devuelve 0 si no existe la lista.
 */
size_t lista_tamanio(lista_t *lista);

/**
 * Obtiene el elemento en la posición dada o NULL si no existe.
 */
void *lista_obtener_elemento(lista_t *lista, int posicion);

/**
 * Saca de la lista el elemento en la posición dada y lo devuelve.
 *
 * Si no existe la posición devuelve null.
 */
void *lista_sacar_de_posicion(lista_t *lista, int posicion);

/**
 * Saca de la lista el elemento pasado por parámetro y lo devuelve.
 *
 * En caso de no existir dicho elemento, devuelve null.
 */
void *lista_sacar_elemento(lista_t *lista, void *elemento);

/**
 * Devuelve la posición del elemento en la lista. Si no existe el elemento
 * devuelve -1.
 *
 */
int lista_buscar_posicion(lista_t *lista, void *elemento);

/**
 * Busca en la lista el primer elemento que cumple con la función de criterio.
 * La función recibe como primer parámetro el elemento de la lista y como
 * segundo parámetro el contexto. Si la función devuelve true, el elemento
 * cumple el criterio.
 *
 * Devuelve el primer elemento que cumple con el criterio o NULL si
 * no hay ninguno.
 */
void *lista_buscar(lista_t *lista, bool (*criterio)(void *, void *),
		   void *contexto);

/**
 * Itera los elementos de la lista aplicando la función f a cada elemento de la
 * misma. Contexto se pasa como segundo parámetro de f.
 *
 * Deja de iterar si la función f devuelve false.
 *
 * Devuelve la cantidad de veces que fue invocada f.
 */
int lista_iterar(lista_t *lista, bool (*f)(void *, void *), void *contexto);

/**
 * Destruye la lista.
 */
void lista_destruir(lista_t *lista);

/**
 * Crea un iterador para la lista.
 * 
 * Devuelve el iterador o NULL en caso de error.
 */
lista_iterador_t *lista_iterador_crear(lista_t *lista);

/**
 * Devuelve true si quedan elementos por recorrer en el iterador.
 * Devuelve false si no quedan elementos por recorrer.
 */
bool lista_iterador_quedan_elementos_por_recorrer(lista_iterador_t *iterador);

/**
 * Avanza el iterador a la siguiente posición.
 *
 * Si no quedan elementos por recorrer, no hace nada.
 */
void lista_iterador_proxima_iteracion(lista_iterador_t *iterador);

/**
 * Devuelve el elemento en la posición actual del iterador.
 *
 * Si no quedan elementos por recorrer, devuelve NULL.
 */
void *lista_iterador_obtener_elemento(lista_iterador_t *iterador);

/**
 * Destruye el iterador.
 *
 * No destruye la lista.
 */
void lista_iterador_destruir(lista_iterador_t *iterador);

#endif // LISTA_H_

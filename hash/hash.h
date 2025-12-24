#ifndef HASH_H_
#define HASH_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct hash hash_t;

/**
 * Crea una tabla de hash de direccionamiento abierto con la capacidad inicial
 * dada. Si la capacidad es menor a 3, se utiliza 3 como capacidad inicial.
 *
 * Devuelve la tabla creada o NULL en caso de error.
 */
hash_t *hash_crear(size_t capacidad_inicial);

/**
 * Igual que hash_crear, pero permite al usuario proveer su propia funcion de hash.
 * La función f no puede ser nula.
 */
hash_t *hash_crear_con_funcion(size_t capacidad_inicial,
			       size_t (*f)(const char *));

/**
 * Inserta una clave en la tabla y asocia el valor pasado por parámetro.
 *
 * Si previamente existe un valor asociado a esa clave y anterior no es nulo, se
 * guarda un puntero al valor anterior en el puntero.
 *
 * Devuelve true si pudo insertar/actualizar el valor.
 */
bool hash_insertar(hash_t *h, const char *clave, void *valor, void **anterior);

/**
 * Quita de la tabla una clave y devuelve el valor asociado.
 *
 * Devuelve el valor asociado o NULL en caso de error.
 */
void *hash_sacar(hash_t *h, const char *clave);

/**
 * Busca en la tabla el valor asociado a una clave. En caso de no existir
 * devuelve NULL.
 */
void *hash_buscar(hash_t *h, const char *clave);

/**
 * Devuelve si existe una clave en la tabla.
 */
bool hash_existe(hash_t *h, const char *clave);

/**
 * Devuelve la cantidad de claves existentes en la tabla.
 */
size_t hash_tamanio(hash_t *h);

/**
 * Destruye la tabla.
 */
void hash_destruir(hash_t *h);

/**
 * Destruye la tabla y aplica el destructor a cada valor presente en la tabla.
 */
void hash_destruir_todo(hash_t *h, void (*destructor)(void *));

/**
 * Aplica la función f a cada clave de la tabla pasando como segundo parámetro el contexto.
 *
 * Deja de iterar cuando f devuelve false.
 *
 * Devuelve la cantidad de veces que fue invocada f.
 */
size_t hash_iterar_claves(hash_t *h, bool (*f)(const char *, void *),
			  void *ctx);

#endif // HASH_H_

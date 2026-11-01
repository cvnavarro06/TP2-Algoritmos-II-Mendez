#ifndef LISTA_H_
#define LISTA_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct lista lista_t;
typedef struct lista_iterador lista_iterador_t;

/*
 * Crea una lista y la devuelve.
 */
lista_t *lista_crear();

/*
 * Devuelve la cantidad de elementos que almacena la lista.
 */
size_t lista_cantidad(lista_t *lista);

/*
 * Devuelve true si la lista está vacía
 */
bool lista_esta_vacia(lista_t *lista);

/*
 * Inserta un dato en la posicion dada de la lista y devuelve true si pudo.
 *
 * Si la posición está mas allá del final de la lista, no se puede insertar.
 *
 * Si no puede insertar devuelve false.
 */
bool lista_insertar(lista_t *lista, void *dato, size_t posicion);

/*
 * Elimina un dato en la posicion dada de la lista y devuelve el elemento eliminado.
 *
 * Si la posición está mas allá del final de la lista, no se puede eliminar y devuelve NULL.
 */
void *lista_eliminar(lista_t *lista, size_t posicion);

/*
 * Reemplaza un dato en la posición dada de la lista y lo devuelve.
 */
void *lista_reemplazar(lista_t *lista, void *dato, size_t posicion);

/*
 * Devuelve el elemento que se encuentra en la posición de la lista.
 */
void *lista_obtener(lista_t *lista, size_t posicion);

/*
 * Busca un elemento en la lista utilizando el comparador. Si lo encuentra devuelve la posición en la que se encuentra.
 *
 * Si no lo encuentra devuelve -1.
 *
 * Si se provee el puntero encontrado, en dicho puntero se almacena el elemento encontrado o NULL en caso de no encontrarse.
 */
int lista_buscar(lista_t *lista, void *buscado,
		 int (*comparador)(void *, void *), void **encontrado);

/*
 * Recorre la lista aplicando la función f. Devuelve la cantidad de veces que fue invocada f.
 *
 * Si f devuevle false, se deja de iterar.
 */
size_t lista_iterar(lista_t *lista, bool (*f)(void *, void *), void *extra);

/*
 * Libera la lista y toda la memoria asociada.
 */
void lista_destruir(lista_t *lista);

/*
 * Libera la lista y toda la memoria asociada.
 *
 * Adicionalmente aplica la función destructora a cada elemento almacenado *
 */
void lista_destruir_todo(lista_t *lista, void (*destructor)(void *));

/*
 * Crea un iterador de lista
 */
lista_iterador_t *lista_iterador_crear(lista_t *lista);

/*
 * Devuelve true si hay mas elementos para iterar
 */
bool lista_iterador_se_puede_iterar(lista_iterador_t *it);

/*
 * Avanza a la siguiente iteración
 */
void lista_iterador_siguiente(lista_iterador_t *it);

/*
 * Devuelve el elemento actual iterado
 */
void *lista_iterador_obtener_elemento(lista_iterador_t *it);

/*
 * Destruye el iterador
 */
void lista_iterador_destruir(lista_iterador_t *it);

#endif // LISTA_H_

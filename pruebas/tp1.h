#ifndef TP1_H_
#define TP1_H_
#include <stdlib.h>
#include <stdbool.h>

enum rareza_pokemon { RAREZA_COMUN, RAREZA_RARO, RAREZA_LEGENDARIO };

typedef struct tp1 tp1_t;

struct pokemon {
	char *nombre;
	int velocidad;
	float peso;
	enum rareza_pokemon rareza;
};

/**
 * Lee un archivo de pokemon según especificado en el enunciado y devuelve un TP1 con dichos pokemon.
 *
 * En caso de error devuelve NULL
 */
tp1_t *tp1_leer_archivo(const char *nombre);

/**
 * Devuelve la cantidad de pokemon leidos en este TP1
 *
 * En caso de error devuelve 0
 *
 */
size_t tp1_cantidad(tp1_t *tp1);

/**
 * Crea un TP1 con la combinación de los pokemon de ambos TPs. En caso de
 * duplicados se toma el pokemon del primer TP1.
 *
 * Devuelve un nuevo TP1 con la combinación de ambos TP1 o NULL en caso de
 * error.
 *
 */
tp1_t *tp1_combinar(tp1_t *tp1_a, tp1_t *tp1_b);

/**
 * Guarda el TP1 a un archivo de texto.
 *
 * Devuelve el mismo TP1 o NULL en caso de error. Si el archivo ya existe se
 * sobreescribe.
 *
 */
tp1_t *tp1_escribir_archivo(tp1_t *tp1, const char *nombre);

/**
 * Busca un pokemon por nombre dentro del TP1 y lo devuelve.
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_pokemon(tp1_t *tp1, const char *nombre);

/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n);

/**
 * Recorre los pokemon del TP1 aplicando la función f a cada uno. Si f devuelve
 * true, sigue iterando.
 *
 * Devuelve la cantidad de veces que se invocó f.
 */
size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra);

/**
 * Destruye el TP1 y toda la memoria asociada
 *
 */
void *tp1_destruir(tp1_t *tp1);

#endif // TP1_H_
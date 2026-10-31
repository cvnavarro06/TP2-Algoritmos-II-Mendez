#ifndef LEER_LINEA_H_
#define LEER_LINEA_H_

#include <stdlib.h>
#include <stdio.h>

/**
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea (\n) o no
 * queden mas caracteres para leer.
 *
 * Devuelve un puntero a un string reservado dinámicamente con malloc a liberar
 * por el usuario utilizando free o NULL en caso de error.
 */
char *leer_linea(FILE* archivo);

/**
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea o no
 * queden mas caracteres por leer. Los caracteres leídos se van insertando en el
 * bloque de memoria apuntada por *ptr de tamaño inicial *tamaño. Dicho bloque
 * debe haber sido reservado por el usuario previa invocación de la función
 * utilizando malloc.
 *
 * Si el tamaño del bloque no es suficiente, se utiliza realloc para agrandar el
 * bloque y se actualiza *tamaño y *ptr.
 *
 * Devuelve la cantidad de caracteres leidos (sin incluír el 0 final) o -1 en
 * caso de error.
 */
int leer_linea_ptr(char **ptr, size_t *tamaño, FILE* archivo);

#endif // LEER_LINEA_H_
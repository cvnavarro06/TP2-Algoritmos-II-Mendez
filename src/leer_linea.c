#include "leer_linea.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR -1
#define CAPACIDAD_MINIMA 10

char *leer_linea(FILE *archivo)
{
	if (archivo == NULL) {
		return NULL;
	}

	int size = 0;
	size_t capacidad = CAPACIDAD_MINIMA;

	char *linea = NULL;
	linea = malloc(capacidad * sizeof(char));

	if (linea == NULL) {
		return NULL;
	}

	//Leo el input mandado por consola
	int c = fgetc(archivo);

	if (c == EOF) {
		free(linea);
		return NULL;
	}

	bool mal_reservado = false;

	while (c != EOF && c != '\n' && !mal_reservado) {
		linea[size++] = (char)c;
		if (size >= capacidad) {
			//Reservo memoría para el proximo loop
			char *aux = { NULL };
			capacidad *= 2;
			aux = realloc(linea, capacidad * sizeof(char));

			if (aux == NULL) {
				mal_reservado = true;
			} else {
				linea = aux;
			}
		}

		if (!mal_reservado) {
			c = fgetc(archivo);
		}
	}

	if (mal_reservado) {
		free(linea);
		return NULL;
	}

	//Cierro el string
	linea[size] = '\0';

	return linea;
}

int leer_linea_ptr(char **ptr, size_t *tamaño, FILE *archivo)
{
	//Verifico el puntero
	if (ptr == NULL || *ptr == NULL || archivo == NULL || *tamaño <= 0) {
		return ERROR;
	}

	size_t total_leidos = 1;

	int c = fgetc(archivo);

	bool mal_reservado = false;

	if (c == EOF) {
		return ERROR;
	}

	while (c != EOF && c != '\n' && !mal_reservado) {
		if (total_leidos >= *tamaño) {
			(*tamaño) *= 2;

			char *aux = realloc(*ptr, (*tamaño) * sizeof(char));

			if (aux == NULL) {
				mal_reservado = true;
			}

			if (!mal_reservado) {
				*ptr = aux;
			}
		}
		if (!mal_reservado) {
			(*ptr)[total_leidos - 1] = (char)c;
			c = fgetc(archivo);
			total_leidos++;
		}
	}

	if (mal_reservado) {
		return ERROR;
	}

	//Cierro el string
	(*ptr)[total_leidos - 1] = '\0';
	(*tamaño)++;

	//Corrijo el total con un -1
	return (int)total_leidos - 1;
}

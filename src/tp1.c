	#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>
#include "tp1.h"
#include "lista.h"
#include "leer_linea.h"

#define PRESICION 0.000001
#define ERR 0
#define CAPACIDAD_INICIAL 10

//El %c extra, es precisamente un extra para hacer una lectura más precisa
const char *FOMRATO_LECTURA = "%m[^,],%d,%f,%c %c";
const char *FORMATO_ESCRITURA = "%s,%d,%.1f,%c\n";
const int LINEAS_ESPERADAS = 4;

const char *MODO_LECTURA = "r";
const char *MODO_ESCRITURA = "w";

const int MAX_VELOCIDAD = 99;
const int MIN_VELOCIDAD = 1;
const int PESO_MIN = 0;

const char RAREZA_COMUN_C = 'C';
const char RAREZA_RARO_C = 'R';
const char RAREZA_LEGENDARIO_C = 'L';

struct tp1 {
	lista_t *pokemones;
};

int no_case_sensitive_strcmp(const char *string_1, const char *string_2)
{
	bool diferentes = false;

	int diferencia = tolower(*string_1) - tolower(*string_2);
	while (*string_1 && *string_2 && !diferentes) {
		//Si es distinto de 0 significa que lo son iguales, por ende no es necesario seguir el bucle
		if (diferencia != 0) {
			diferentes = true;
		} else {
			//Avanzo haciendo aritmetica de punteros
			string_1++;
			string_2++;
			diferencia = tolower(*string_1) - tolower(*string_2);
		}
	}

	if (diferentes) {
		return diferencia;
	}

	return diferencia;
}

char *duplicar_string(char *string)
{
	if (string == NULL) {
		return NULL;
	}

	size_t largo_string = strlen(string) + 1;

	char *nuevo_string = malloc(largo_string * sizeof(char));

	if (nuevo_string != NULL) {
		memcpy(nuevo_string, string, largo_string);
	}

	return nuevo_string;
}

//A cambiar qsort
void ordenar_pokemones(struct pokemon *pokemones, size_t tope)
{
	struct pokemon aux;
	size_t j;
	for (size_t i = 1; i < tope; i++) {
		j = i;
		aux = pokemones[i];
		while ((j > 0) &&
		       no_case_sensitive_strcmp(pokemones[j - 1].nombre,
						aux.nombre) > 0) {
			pokemones[j] = pokemones[j - 1];
			j--;
		}
		pokemones[j] = aux;
	}
}

//A cambiar
void eliminar_duplicados(tp1_t *file)
{
//	for (int i = 1; i < (int)file->cantidad; i++) {
//		if (no_case_sensitive_strcmp(file->pokemones[i].nombre,
//					     file->pokemones[i - 1].nombre) ==
//		    0) {
//			free(file->pokemones[i].nombre);
//
//			for (int j = i; j < (int)file->cantidad - 1; j++) {
//				file->pokemones[j] = file->pokemones[j + 1];
//			}
//			file->cantidad--;
//
//			//Resto un i para evaluar si el siguiente pokemon es igual al anterior (Pueden ser varios duplicados)
//			i--;
//		}
//	}
}

struct pokemon *pokemon_crear(char *nombre, int velocidad, float peso, enum rareza_pokemon rareza) {
    struct pokemon *p = malloc(sizeof(struct pokemon));
    if (!p) return NULL;

    p->nombre = nombre;
    p->velocidad = velocidad;
    p->peso = peso;
    p->rareza = rareza;

    return p;
}


/**
 * Lee un archivo de pokemon según especificado en el enunciado y devuelve un TP1 con dichos pokemon.
 *
 * En caso de error devuelve NULL
 */
tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (nombre == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, MODO_LECTURA);

	if (archivo == NULL) {
		return NULL;
	}

	tp1_t *file = malloc(sizeof(tp1_t));

	if (file == NULL) {
		fclose(archivo);
		return NULL;
	}

	file->cantidad = 0;

	file->pokemones = lista_crear();

	if (!file->pokemones) {
		lista_destruir(file->pokemones);
		free(file);
		fclose(archivo);
		return NULL;
	}

	int velocidad;
	float peso;
	char rareza;
	char otro;

	bool err = false;

	char *linea = leer_linea(archivo);

	while (linea != NULL && !err) {
		char *name = NULL;

		/*Tareas:
			* Hacer un sorter de listas para ir ordenando mientras inserto
			* Busqueda de repetidos (Hacer con lo) 
		*/
		
		//Ya cargué el dato por ende libero el "linea"
		free(linea);
		linea = leer_linea(archivo);
	}

	if (err) {
		tp1_destruir(file);
		fclose(archivo);
		return NULL;
	} else {
		ordenar_pokemones(file->pokemones, file->cantidad);

		eliminar_duplicados(file);
	}

	fclose(archivo);

	return file;
}

/**
 * Devuelve la cantidad de pokemon leidos en este TP1
 *
 * En caso de error devuelve 0
 *
 */
size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1)
		return ERR;

	return lista_cantidad(tp1->pokemones);
}

/**
 * Crea un TP1 con la combinación de los pokemon de ambos TPs. En caso de
 * duplicados se toma el pokemon del primer TP1.
 *
 * Devuelve un nuevo TP1 con la combinación de ambos TP1 o NULL en caso de
 * error.
 *
 */
tp1_t *tp1_combinar(tp1_t *tp1_a, tp1_t *tp1_b)
{
	//HACER UN MERGE

	if (tp1_a == NULL || tp1_b == NULL)
		return NULL;
	

	//Reservo un tp1
	tp1_t *nuevo = malloc(sizeof(tp1_t));

	if (!nuevo) 
		return NULL;
	
	nuevo->pokemones = lista_crear();

	if (!nuevo->pokemones)
		return NULL;

	

	return nuevo;
}

char tipo_rareza(struct pokemon *p)
{
	char rareza_e = p->rareza;

	char rareza_c = RAREZA_COMUN_C;

	if (rareza_e == RAREZA_COMUN) {
		rareza_c = RAREZA_COMUN_C;
	} else if (rareza_e == RAREZA_RARO) {
		rareza_c = RAREZA_RARO_C;
	} else if (rareza_e == RAREZA_LEGENDARIO) {
		rareza_c = RAREZA_LEGENDARIO_C;
	}

	return rareza_c;
}

bool escribir_en_archivo(void *pokemon, void *archivo)
{
	if (!archivo || !pokemon)
		return false;

	struct pokemon *p = (struct pokemon*)pokemon;
	FILE *f = (FILE *)archivo;

	char r = tipo_rareza(p);

	fprintf(f, FORMATO_ESCRITURA, p->nombre, 
			p->peso, p->velocidad, r);
	
	return true;
}

/**
 * Guarda el TP1 a un archivo de texto.
 *
 * Devuelve el mismo TP1 o NULL en caso de error. Si el archivo ya existe se
 * sobreescribe.
 *
 */
tp1_t *tp1_escribir_archivo(tp1_t *tp1, const char *nombre)
{
	if (!tp1 || !nombre) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, MODO_ESCRITURA);

	if (!archivo) {
		return NULL;
	}

	lista_iterar(tp1->pokemones, escribir_en_archivo, archivo);

	fclose(archivo);

	return tp1;
}


bool buscar_pokemon(void *pokemon, void *nombre)
{
	if (!nombre || !pokemon)
		return false;
	
	struct pokemon *p = (struct pokemon *)pokemon;
	char *nom = (char *)nombre;

	bool no_encontre = true;

	if (strcmp(p->nombre, nombre) == 0) {
		no_encontre = false;
	}

	return no_encontre;
}

/**
 * Busca un pokemon por nombre dentro del TP1 y lo devuelve.
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_pokemon(tp1_t *tp1, const char *nombre)
{
	if (!tp1 || !nombre) {
		return NULL;
	}

	size_t posicion = lista_iterar(tp1->pokemones, buscar_pokemon, nombre);

	return lista_obtener(tp1->pokemones, posicion);
}


/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n)
{
	if (!tp1 || n >= lista_cantidad(tp1->pokemones))
		return NULL;

	return lista_obtener(tp1->pokemones, n);
}

/**
 * Recorre los pokemon del TP1 aplicando la función f a cada uno. Si f devuelve
 * true, sigue iterando.
 *
 * Devuelve la cantidad de veces que se invocó f.
 */
size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra)
{
	if (!tp1 || !f || extra) {
		return 0;
	}

	return lista_iterar(tp1->pokemones, f, extra);
}

void destruir_pokemon(void *pokemon)
{
	struct pokemon *p = (struct pokemon *)pokemon;

	if (!p) {
		free(p->nombre);
		free(p);
	}
}

/**
 * Destruye el TP1 y toda la memoria asociada
 *
 */
void *tp1_destruir(tp1_t *tp1)
{
	if (!tp1) {
		return NULL;
	}

	lista_destruir_todo(tp1->pokemones, destruir_pokemon);

	free(tp1);

	return NULL;
}

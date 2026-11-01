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
	if (!string) {
		return NULL;
	}

	size_t largo_string = strlen(string) + 1;

	char *nuevo_string = malloc(largo_string * sizeof(char));

	if (nuevo_string) {
		memcpy(nuevo_string, string, largo_string);
	}

	return nuevo_string;
}

int comparar_pokemones(struct pokemon *poke1, struct pokemon *poke2)
{
	return no_case_sensitive_strcmp(poke1->nombre, poke2->nombre);
}

/*
 * Se ordenarán los pokemones de menor a mayor utilizando inserción ordenada.
 *
 * En caso de duplicado se liberará el pokemon junto a su memoria asociada.
*/
void insertar_pokemon_ordenado(lista_t *lista, struct pokemon *p) 
{   	
	
	lista_iterador_t *it = lista_iterador_crear(lista);
	
    if (!it) 
		return;
	
	size_t posicion = 0;

	bool pos_encontrada = false;

	bool duplicado = false;

    while (lista_iterador_se_puede_iterar(it) && !pos_encontrada && !duplicado) {
        struct pokemon *actual = (struct pokemon *)lista_iterador_obtener_elemento(it);
        
		int cmp = no_case_sensitive_strcmp(p->nombre, actual->nombre);

        if (cmp == 0) {
			duplicado = true;
		} else if (cmp < 0) {
			pos_encontrada = true; 
		}

        if (!pos_encontrada && !duplicado) {
        	lista_iterador_siguiente(it);
        	posicion++;
		}
    }
    
    lista_iterador_destruir(it);
    
    if (duplicado) {
		free(p->nombre);	
		free(p);	
	}
	lista_insertar(lista, p, posicion);
}

enum rareza_pokemon rareza_enum(char rareza)
{
	enum rareza_pokemon e_rareza = RAREZA_COMUN;

	if (rareza == RAREZA_COMUN_C)
		e_rareza = RAREZA_COMUN;
	else if (rareza == RAREZA_RARO_C)
		e_rareza = RAREZA_RARO;
	else if (rareza == RAREZA_LEGENDARIO_C)
		e_rareza = RAREZA_LEGENDARIO;

	return e_rareza;

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

struct pokemon *pokemon_crear(char *nombre, int velocidad, float peso, char rareza) 
{    
	struct pokemon *p = malloc(sizeof(struct pokemon));
    
	if (!p) 
		return NULL;

    p->nombre = duplicar_string(nombre);

	if (!p->nombre) 
		return NULL;
	
    p->velocidad = velocidad;
    p->peso = peso; 
    p->rareza = rareza_enum(rareza);

    return p;
}

bool insercion_al_tp1(tp1_t *t, FILE* archivo)
{
	int velocidad;
	float peso;
	char rareza;
	char otro;

	bool err = false;

	char *linea = leer_linea(archivo);

	while (linea && !err) {
		char *nombre = NULL;

		int leido = sscanf(linea, FOMRATO_LECTURA, &nombre, &velocidad, &peso, &rareza, &otro);

		if (leido == 4) {
			struct pokemon *poke = pokemon_crear(nombre, velocidad, peso, rareza);

			if (!poke) err = true;

			if (!err) insertar_pokemon_ordenado(t->pokemones, poke);
		}
		free(nombre); //Internamente, estoy duplicando el string en otro bloque de memoria, por ende ya no me sirve este bloque.
		free(linea);
		
		if (!err)
			linea = leer_linea(archivo);
	}	

	return err;
}

/**
 * Lee un archivo de pokemon según especificado en el enunciado y devuelve un TP1 con dichos pokemon.
 *
 * En caso de error devuelve NULL
 */
tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (!nombre) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, MODO_LECTURA);

	if (!archivo) {
		return NULL;
	}

	tp1_t *t = malloc(sizeof(tp1_t));

	if (!t) {
		tp1_destruir(t);
		fclose(archivo);
		return NULL;
	}

	t->pokemones = lista_crear();

	if (!t->pokemones) {
		tp1_destruir(t);
		fclose(archivo);
		return NULL;
	}

	bool err = insercion_al_tp1(t, archivo);

	if (err) {
		tp1_destruir(t);
		fclose(archivo);
		return NULL;
	}

	fclose(archivo);

	return t;
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

bool resto_tps(lista_iterador_t *t1, lista_iterador_t *t2, tp1_t *nuevo)
{
	bool err = false;

	size_t posicion = lista_cantidad(nuevo->pokemones);

	while (lista_iterador_se_puede_iterar(t1) && !err) {
        struct pokemon *p_a = (struct pokemon *)lista_iterador_obtener_elemento(t1);

		char rareza_a = tipo_rareza(p_a);
		
        struct pokemon *a_insertar = pokemon_crear(p_a->nombre, p_a->velocidad, p_a->peso, rareza_a);

        if (!a_insertar || !lista_insertar(nuevo->pokemones, a_insertar, posicion)) {
            err = true;
        } else {
            lista_iterador_siguiente(t1);
        }
    }

    while (lista_iterador_se_puede_iterar(t2) && !err) {
        struct pokemon *p_b = (struct pokemon *)lista_iterador_obtener_elemento(t2);

		char rareza_b = tipo_rareza(p_b);
		
        struct pokemon *a_insertar = pokemon_crear(p_b->nombre, p_b->velocidad, p_b->peso, rareza_b);

        if (!a_insertar || !lista_insertar(nuevo->pokemones, a_insertar, posicion)) {
            err = true;
        } else {
            lista_iterador_siguiente(t2);
        }
    }

	return err;
}

bool combinacion_de_tps(lista_iterador_t *t1, lista_iterador_t *t2, tp1_t *nuevo)
{
	bool err = false;

    while (lista_iterador_se_puede_iterar(t1) && lista_iterador_se_puede_iterar(t2) && !err) {
        struct pokemon *p_a = (struct pokemon *)lista_iterador_obtener_elemento(t1);
        struct pokemon *p_b = (struct pokemon *)lista_iterador_obtener_elemento(t2);

		size_t posicion = lista_cantidad(nuevo->pokemones);

        int cmp = no_case_sensitive_strcmp(p_a->nombre, p_b->nombre);
        struct pokemon *a_insertar = NULL;

		char rareza_a = tipo_rareza(p_a);
		char rareza_b = tipo_rareza(p_b);

        if (cmp < 0) {
            a_insertar = pokemon_crear(p_a->nombre, p_a->velocidad, p_a->peso, rareza_a);
            lista_iterador_siguiente(t1);
        } else if (cmp > 0) {
            a_insertar = pokemon_crear(p_b->nombre, p_b->velocidad, p_b->peso, rareza_b);
            lista_iterador_siguiente(t2);
        } else {
            a_insertar = pokemon_crear(p_a->nombre, p_a->velocidad, p_a->peso, rareza_a);

            lista_iterador_siguiente(t1);
            lista_iterador_siguiente(t2); // Se descarta el de tp1_b
        }

        if (!a_insertar || !lista_insertar(nuevo->pokemones, a_insertar, posicion)) {
            err = true;
        }
    }

	err = resto_tps(t1, t2, nuevo);

	return err;
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
    if (!tp1_a || !tp1_b)
        return NULL;

    tp1_t *nuevo = malloc(sizeof(tp1_t));
    if (!nuevo) 
        return NULL;

    nuevo->pokemones = lista_crear();
    if (!nuevo->pokemones) {
        free(nuevo);
        return NULL;
    }

    lista_iterador_t *t1 = lista_iterador_crear(tp1_a->pokemones);

    lista_iterador_t *t2 = lista_iterador_crear(tp1_b->pokemones);

    if (!t1 || !t2) { //Si 1 de los 2 es NULL no me importa ya que no pasará la función de destruir
        if (t1) lista_iterador_destruir(t1);
        if (t2) lista_iterador_destruir(t2);
		tp1_destruir(nuevo);
        return NULL;
    }

	bool error = combinacion_de_tps(t1, t2, nuevo);

    lista_iterador_destruir(t1);
    lista_iterador_destruir(t2);

    if (error) {
        tp1_destruir(nuevo);
        return NULL;
    }

    return nuevo;
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

	lista_iterador_t *it = lista_iterador_crear(tp1->pokemones);

	if (!it) return NULL;

	bool encontrado = false;

	struct pokemon *p = NULL;

	while (lista_iterador_se_puede_iterar(it) && !encontrado) {
		p = lista_iterador_obtener_elemento(it);

		if (no_case_sensitive_strcmp(p->nombre, nombre) == 0) {
			encontrado = true;
		}
	}

	lista_iterador_destruir(it);

	return p;
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
	if (!tp1 || !f || !extra) {
		return 0;
	}

	bool fin = false;

	size_t contador = 0;

	size_t cantidad = lista_cantidad(tp1->pokemones);

	for (int i = 0; i < cantidad && !fin; i++) {
		contador++;
		struct pokemon *p = lista_obtener(tp1->pokemones, i);

		if (!(f)(p, extra)) {
			fin = true;
		}
	}

	return contador;
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

#include "extra/engine.h"
#include <stdio.h>
#include <string.h>

#define EXITO

#define ERROR -1
#define ARCHIVO 2
#define RUTA 3

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Mal input");
		return ERROR;
	}
	
	if (argc == 2) {
		//Solo manejo acá la opción de jugar, como esta en la consola indico que debe
		//De estar en la web, por ende lo devuelvo
		return EXITO; 
	}


	return EXITO;
}
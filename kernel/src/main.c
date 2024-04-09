#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "main.h"

t_config* config;

int main(int argc, char* argv[]) {
    decir_hola("Kernel");

    config = crearConfig(argv[1]);

    iniciarNecesidades();
	
	char nombre[50];
	scanf("%s", nombre);

    return 0;
}

void *conectarMemoria(void *parametro){
	conectarModuloMemoria(parametro);
	return NULL;
}

void *conectarFilesystem(void *parametro){
	conectarModuloFilesystem(parametro);
	return NULL;
}

void *conectarCPU(void *parametro){
	conectarModuloCPU(parametro);
	return NULL;
}

void *conectarCPUInterrupt(void *parametro){
	conectarModuloCPUInterrupt(parametro);
	return NULL;
}

t_config *crearConfig(char* configPath){
	t_config* config;

	config = config_create(configPath);

	if(config != NULL)
		return config;

	printf("La configuracion no se pudo crear exitosamente");
	exit(2);

	return config;
}
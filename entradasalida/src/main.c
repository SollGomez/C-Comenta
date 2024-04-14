#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "main.h"

t_config* config;

int main(int argc, char* argv[]) {
    decir_hola("una Interfaz de Entrada/Salida");

    config = crearConfig(argv[1]);
    
	for(int i=0; i<4; i++)
	{
		conectarKernel("KERNEL");
		conectarMemoria("MEMORIA");
		contadorDispositivosIO++;
	}

	char nombre[50];
	scanf("%s", nombre);

    return 0;
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
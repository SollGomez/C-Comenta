#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include <clean.h>
#include "main.h"

t_config* config;

int main(int argc, char* argv[]) {
    decir_hola("una Interfaz de Entrada/Salida");
  
    config = crearConfig(argv[1]);
    init_loggers_config(argv[1]);
    cargar_configuracion();
    
    

	conectarKernel("KERNEL");
	conectarMemoria("MEMORIA");
	
	
	
	char nombre[50];
	scanf("%s", nombre);

	cerrarPrograma();
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
#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "main.h"

char *PUERTO_DISPATCH;
char *PUERTO_INTERRUPT;
t_config *config;
t_log* info_logger;
t_log* error_logger;

int main(int argc, char* argv[]) {
    decir_hola("CPU");
    
    config = crearConfig(argv[1]);
    info_logger = log_create("info_logger.log","Cpu", true, LOG_LEVEL_INFO);
	error_logger = log_create("error_logger.log","Cpu", true, LOG_LEVEL_ERROR);
	PUERTO_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
	PUERTO_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");

    pthread_t tid[3];
	pthread_create(&tid[1], NULL, conectarMemoria, NULL);

    pthread_join(tid[1], NULL);

    // pthread_create(&tid[0], NULL, recibir, NULL);
	// pthread_create(&tid[2], NULL, recibirInterrupt, NULL);

	// pthread_join(tid[0], NULL);
	// pthread_join(tid[2], NULL);

    return 0;
}

void *recibir(){
	recibirConexion(PUERTO_DISPATCH);
	return NULL;
}

void *recibirInterrupt(){
	recibirConexionInterrupt(PUERTO_INTERRUPT);
	return NULL;
}

void *conectarMemoria(){
	conectarModulo("MEMORIA");
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
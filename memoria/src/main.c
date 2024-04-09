#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "main.h"

t_log* info_logger;
char *PUERTO;
t_config *config;


int main(int argc, char* argv[]) {
    decir_hola("Memoria");
    
    info_logger = log_create("info_logger.log","Memory", true, LOG_LEVEL_INFO);
    pthread_t tid;
    log_info(info_logger, "%s",argv[1]);
    config = crearConfig(argv[1]);
	PUERTO = config_get_string_value(config, "PUERTO_ESCUCHA");

    pthread_create(&tid, NULL, recibir, NULL);
	pthread_join(tid, NULL);

    return 0;
}

void *recibir(){
	recibirConexion(PUERTO);
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
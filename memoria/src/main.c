#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "main.h"
#include <signal.h>

t_log* info_logger;
char *PUERTO;
t_config *config;


int main(int argc, char* argv[]) {
    decir_hola("Memoria");

	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error al configurar el manejador de señal");
        return EXIT_FAILURE;
    }
    
    info_logger = log_create("info_logger.log","Memory", true, LOG_LEVEL_INFO);
    pthread_t tid;
    
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

void sigint_handler(int sig) {
    printf("\nSe ha recibido la señal SIGINT (Ctrl+C). Cerrando sockets...\n");
    // Aquí puedes cerrar tus sockets u realizar otras tareas necesarias
	close(cpu_fd);
	close(memoria_fd);
    exit(EXIT_SUCCESS); // Puedes modificar esto según sea necesario
}
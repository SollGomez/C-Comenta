#include <main.h>

t_config* config;

int main(int argc, char* argv[]) {

	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error al configurar el manejador de señal");
        return EXIT_FAILURE;
    }
  
    config = crearConfig(argv[1]);
    init_loggers_config(argv[1]);
    cargar_configuracion();
	if(strcmp("DIALFS", cfg_entradaSalida->TIPO_INTERFAZ) == 0)
		crearEstructurasFs();


		
	
	pthread_t kernel;
	pthread_t memoria;

    pthread_create(&kernel, NULL, iniciarKernel, NULL);

	if(cfg_entradaSalida->TIPO_INTERFAZ_INT != 3) {
		pthread_create(&memoria, NULL, iniciarMemoria, NULL);
		pthread_join(memoria, NULL);
	}

	pthread_join(kernel, NULL);

    return 0;
}

void sigint_handler(int sig) {
    printf("\nSe ha recibido la señal SIGINT (Ctrl+C). Cerrando sockets...\n");
	cerrarPrograma();
    exit(EXIT_SUCCESS); 
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
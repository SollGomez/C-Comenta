#include <main.h>

t_config* config;

int main(int argc, char* argv[]) {
  
    config = crearConfig(argv[1]);
    init_loggers_config(argv[1]);
    cargar_configuracion();
	crearListas();
	crearSemaforos();
	iniciarAtencionPeticiones();
	crearEstructurasFs();


	crearArchivo("pruebaLoca.txt");


	// pthread_t kernel;
	// pthread_t memoria;

    // pthread_create(&kernel, NULL, iniciarKernel, NULL);

	// if(cfg_entradaSalida->TIPO_INTERFAZ_INT != 3) {
	// 	pthread_create(&memoria, NULL, iniciarMemoria, NULL);
	// 	pthread_join(memoria, NULL);
	// }

	// pthread_join(kernel, NULL);

	// cerrarPrograma();
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
#include <main.h>

t_config* config;

int main(int argc, char* argv[]) {
  
    config = crearConfig(argv[1]);
    init_loggers_config(argv[1]);
    cargar_configuracion();
	if(strcmp(cfg_entradaSalida->TIPO_INTERFAZ, "DIALFS")){
		crearEstructurasFs();
	}

	//crearArchivo("duhndaud.txt");
	//eliminarArchivo("duhndaud.txt");
	//manejarInterfazStdin(2, 30);
	
/*
	//listaDeArchivos = list_create();
	crearArchivo("pruebaLoca.txt");
	//list_add(listaDeArchivos, "pruebaLoca.txt");
	truncarArchivo("pruebaLoca.txt", 64); 
	escribirArchivo("hola ", "HOLA COMO ESTAS HOLA COMO ESTAS HOLA COMO ESTAS HOLA COMO ESTAS", 0, 64);
	crearArchivo("pruebaLoca2.txt");
	//list_add(listaDeArchivos, "pruebaLoca2.txt");

	//msync(bitmap, cfg_entradaSalida->BLOCK_COUNT, MS_SYNC);
	truncarArchivo("pruebaLoca2.txt", 128);
	truncarArchivo("pruebaLoca.txt", 128);
	//truncarArchivo("pruebaLoca.txt", 0);
	// for(int i=0; i<bitarray_get_max_bit(bitmap); i++){
    //    log_info(info_logger, "%d", bitarray_test_bit(bitmap, i));
    // }
*/
	
	pthread_t kernel;
	pthread_t memoria;

    pthread_create(&kernel, NULL, iniciarKernel, NULL);

	if(cfg_entradaSalida->TIPO_INTERFAZ_INT != 3) {
		pthread_create(&memoria, NULL, iniciarMemoria, NULL);
		pthread_join(memoria, NULL);
	}

	pthread_join(kernel, NULL);

	//cerrarPrograma();
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
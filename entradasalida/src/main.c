#include <main.h>

t_config* config;

int main(int argc, char* argv[]) {
  
    config = crearConfig(argv[1]);
    init_loggers_config(argv[1]);
    cargar_configuracion();
	crearEstructurasFs();

	uint32_t pid = (uint32_t*) 10;
	listaDeArchivos = list_create();
	crearArchivo("archivo.txt");
	crearArchivo("pizzas.txt");
	crearArchivo("tomates.txt");
	crearArchivo("nose.txt");
	list_add(listaDeArchivos, "archivo.txt");
	list_add(listaDeArchivos, "pizzas.txt");
	list_add(listaDeArchivos, "tomates.txt");
	list_add(listaDeArchivos, "nose.txt");

	//truncarArchivo("archivo.txt", 64, pid); 
	escribirArchivo("hola ", "HOLA COMO ESTAS HOLA COMO ESTAS HOLA COMO ESTAS HOLA COMO ESTAS", 0, 64);
	mostrarBitmap();
	log_error(error_logger, "Truncando <tomates> a 128");	
	truncarArchivo("tomates.txt", 128, pid);
	mostrarBitmap();
	log_error(error_logger, "Truncando <archivo> a 192");
	truncarArchivo("archivo.txt", 192, pid);
	mostrarBitmap();
	log_error(error_logger, "Truncando <pizzas> a 64");
	truncarArchivo("pizzas.txt", 64, pid);
	mostrarBitmap();
	log_error(error_logger, "Truncando <archivo> a 64");
	truncarArchivo("archivo.txt", 64, pid);
	mostrarBitmap();
	log_error(error_logger, "Truncando <pizzas> a 128");
	truncarArchivo("pizzas.txt", 128, pid);
	mostrarBitmap();


	t_config* config = config_create("/home/utnso/dialfs/pizzas.txt");

	int valor = config_get_int_value(config, "BLOQUE_INICIAL");
	
	printf("VALOR: %d", valor);

	//msync(bitmap, cfg_entradaSalida->BLOCK_COUNT, MS_SYNC);
	//truncarArchivo("pruebaLoca2.txt", 65408);
	//truncarArchivo("pruebaLoca.txt", 0);
		
	/*
	pthread_t kernel;
	pthread_t memoria;

    pthread_create(&kernel, NULL, iniciarKernel, NULL);

	if(cfg_entradaSalida->TIPO_INTERFAZ_INT != 3) {
		pthread_create(&memoria, NULL, iniciarMemoria, NULL);
		pthread_join(memoria, NULL);
	}

	pthread_join(kernel, NULL);

	cerrarPrograma();
	*/
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
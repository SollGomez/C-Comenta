#include "main.h"

char *PUERTO;
char *algoritmo;
char* PATH_INSTRUCCIONES;
t_config *config;
t_list* tablaGeneral;
InstruccionPseudo* instrucciones;
t_log* info_logger;
t_list *instruccionesEnMemoria;

int main(int argc, char* argv[]) {
	// liberar_conexion(cpu_fd);
	// liberar_conexion(kernel_fd);
	// liberar_conexion(filesystem_fd);
	info_logger = log_create("info_logger.log","Memory", true, LOG_LEVEL_INFO);
	pthread_t tid[2];
	config = crearConfig(argv[1]);
	PUERTO = config_get_string_value(config, "PUERTO_ESCUCHA");
	algoritmo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	PATH_INSTRUCCIONES = config_get_string_value(config, "PATH_INSTRUCCIONES");
	RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");
	tablaGeneral = list_create();
	instruccionesEnMemoria = list_create();


	iniciarMemoria();

	pruebas();

	//pthread_create(&tid[0], NULL, recibir, NULL);
	//pthread_join(tid[0], NULL);


	config_destroy(config);
	list_destroy(tablaGeneral);
	log_destroy(info_logger);
	free(espacio_contiguo);
	return EXIT_SUCCESS;
}

// void *recibir(){
// 	recibirConexion(PUERTO);
// 	return NULL;
// }

void pruebas(){

	int marco;
	int marco2;
	int marco3;
	TablaDePaginas* tabla;
	
	printf("La cantidad de marcos es: %u\n", CANT_MARCOS);
	printf("TAM_PAGINA: %u\n", TAM_PAGINA);
	printf("TAM_MEMORIA: %u\n", TAM_MEMORIA);
	// marco = buscarMarcoLibre();
	// printf("El primer marco libre es (debe ser 0): %u\n", marco);
	// marcarMarcoOcupado(0);
	// marco2 = buscarMarcoLibre();
	// printf("El segundo marco libre es (debe ser 1): %u\n", marco2);
	// marcarMarcoLibre(0);
	// marco3 = buscarMarcoLibre();
	// printf("El tercer marco libre es (debe ser 0): %u\n", marco);
	TablaDePaginas* tabla1;
 	printf("TAMAÑO TABLAGENERAL ANTES DE AGREGAR UNA TABLA: %d\n", list_size(tablaGeneral));
	list_add(tablaGeneral, tabla1);
	printf("TAMAÑO TABLAGENERAL DESPUES DE AGREGAR UNA TABLA: %d\n", list_size(tablaGeneral));
	void* element = list_remove(tablaGeneral, 0);
	printf("TAMAÑO TABLAGENERAL AL ELIMINAR UNA TABLA: %d\n", list_size(tablaGeneral));


	crearTablaPaginasProceso(1,64);
	tabla = obtenerTablaPorPID(1);
	printf("La tabla de paginas del proceso con PID 1 tiene estas paginas: %d\n", list_size(tabla->paginas));
	Pagina* pagina1 = list_get(tabla->paginas, 0);
	Pagina* pagina2 = list_get(tabla->paginas, 1);
	printf("El marco de la pagina 1 del proceso 1 es: %d\n", pagina1->marco);
	printf("El marco de la pagina 2 del proceso 1 es: %d\n", pagina2->marco);
	finalizacionDeProceso(1);
	printf("El tamaño de la TablaGeneral es (debe ser 0): %u\n", list_size(tablaGeneral));
}
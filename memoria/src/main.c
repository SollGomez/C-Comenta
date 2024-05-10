#include "main.h"

char *PUERTO;
char *algoritmo;
char* PATH_INSTRUCCIONES;
t_config *config;
t_list* tablaGeneral;
InstruccionPseudo* instrucciones;
t_log* info_logger;
t_list *instruccionesDeProcesos;


void pruebaSol();
void pruebaPeque();
void prueba();

int main(int argc, char* argv[]) {
	// liberar_conexion(cpu_fd);
	// liberar_conexion(kernel_fd);
	// liberar_conexion(filesystem_fd);
	info_logger = log_create("info_logger.log","Memory", true, LOG_LEVEL_INFO);
	// pthread_t tid[2];
	config = crearConfig(argv[1]);
	// PUERTO = config_get_string_value(config, "PUERTO_ESCUCHA");
	// PATH_INSTRUCCIONES = config_get_string_value(config, "PATH_INSTRUCCIONES");
	// RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");
	tablaGeneral = list_create();

	// instruccionesDeProcesos = list_create();

	iniciarMemoria();

	prueba();

	//pthread_create(&tid[0], NULL, recibir, NULL);
	//pthread_join(tid[0], NULL);

	config_destroy(config);
//	list_destroy(tablaGeneral);
//	log_destroy(info_logger);
//	free(espacio_contiguo);
	//return EXIT_SUCCESS;
//	list_destroy(instruccionesDeProcesos);
	return 0;
}

// void *recibir(){
// 	recibirConexion(PUERTO);
// 	return NULL;
// }

void pruebaSol(){
	printf("Hola, ando :)\n");

	GuardarInstrucsDeProceso(1, "script_io_basico_1");
	GuardarInstrucsDeProceso(2, "script_io_basico_2");
	GuardarInstrucsDeProceso(3, "script_io_basico_3");
	GuardarInstrucsDeProceso(4, "script_solo_cpu_1");
	GuardarInstrucsDeProceso(5, "script_solo_cpu_2");
	GuardarInstrucsDeProceso(6, "script_solo_cpu_3");
	GuardarInstrucsDeProceso(7, "script_solo_cpu_4");

	for(int i=0; i<7; i++){
		Proceso* proceso = list_get(instruccionesDeProcesos, i);
		printf("\nProceso: %d\n", proceso->pid);

		for(int j=0; j<=list_size(proceso->instrucciones)-1; j++){
			Instruccion* inst = list_get(proceso->instrucciones, j);
			printf("%s ", inst->id);

			int cantParametros = inst->cantidadParametros;
			switch (cantParametros){
			case 5: 
				printf("%s ", inst->param1);
				printf("%s ", inst->param2);
				printf("%s ", inst->param3);
				printf("%s ", inst->param4);
				printf("%s ", inst->param5);
				break;

			case 3:
				printf("%s ", inst->param1);
				printf("%s ", inst->param2);
				printf("%s", inst->param3);
				break;

			case 2:
				printf("%s ", inst->param1);
				printf("%s ", inst->param2);
				break;

			case 1:
				printf("%s ", inst->param1);
				break;

			case 0:
				printf("\n");
				break;
			}
		}
	}
}	

void prueba(){
	uint32_t rta = 0;

	crearTablaPaginasProceso(1,64);
	crearTablaPaginasProceso(2,64);

	TablaDePaginas* tabla1 = obtenerTablaPorPID(1);
	TablaDePaginas* tabla2 = obtenerTablaPorPID(2);

	printf("La tabla de paginas del proceso con PID 1 tiene estas paginas: %d\n", list_size(tabla1->paginas));
	rta = resizeProceso(1, 128);
	printf("La tabla de paginas del proceso con PID 1 tiene estas paginas despues del resize: %d\n", list_size(tabla1->paginas));
	printf("La tabla de paginas del proceso con PID 2 tiene estas paginas: %d\n", list_size(tabla2->paginas));
	resizeProceso(2, 32);
	printf("La tabla de paginas del proceso con PID 2 tiene estas paginas despues del resize: %d\n", list_size(tabla2->paginas));

	rta = resizeProceso(1, 128);
	if(rta == 0){
		printf("se pudo hacer resize\n");
	}else
		printf("No se pudo hacer resize\n");
}

void pruebaPeque(){

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
	uint32_t pagsVacias = cantidadMarcosVacios();
	printf("Pags vacias: %d\n", pagsVacias);

	finalizacionDeProceso(1);
	printf("El tamaño de la TablaGeneral es (debe ser 0): %u\n", list_size(tablaGeneral));
}
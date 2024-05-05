#include <comunicacion.h>

int contadorDispositivosIO=0;
int memoria_fd;
int kernel_fd;

t_list* lista_peticiones_pendientes;
pthread_mutex_t mutex_recvKernel;
pthread_mutex_t mutex_peticiones_pendientes;
sem_t sem_contador_peticiones;

int conectarMemoria(char *modulo){
	char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	strcpy(charAux, modulo);
    t_log* loggerIOMem;
	loggerIOMem= iniciar_logger(strcat(charAux,".log"));

	strcpy(charAux, modulo);

	strcpy(parametro, "IP_");
    strcat(parametro, charAux);
	ip=config_get_string_value(configuracionEntradasalida,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(configuracionEntradasalida,parametro);

	log_info(loggerIOMem, "IP=%s\n", ip);
	log_info(loggerIOMem, "PUERTO=%s\n", puerto);

	memoria_fd= crear_conexion(loggerIOMem, "Conecto dispositivo IO a Memoria",ip, puerto);
	

	int32_t handshakeEntradasalida;

	handshakeEntradasalida = cfg_entradaSalida->TIPO_INTERFAZ_INT;

	send(memoria_fd, &handshakeEntradasalida, sizeof(int32_t), 0);

	log_destroy(loggerIOMem);

	return memoria_fd;
}

int conectarKernel(char *modulo){

	char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	strcpy(charAux, modulo);
    t_log* loggerIOKernel;
	loggerIOKernel= iniciar_logger(strcat(charAux,".log"));

	strcpy(charAux, modulo);

	strcpy(parametro, "IP_");
    strcat(parametro, charAux);
	ip=config_get_string_value(configuracionEntradasalida,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(configuracionEntradasalida,parametro);

	log_info(loggerIOKernel, "IP=%s\n", ip);
	log_info(loggerIOKernel, "PUERTO=%s\n", puerto);

	kernel_fd = crear_conexion(loggerIOKernel, "Conecto dispositivo IO a Kernel",ip, puerto);

	int32_t handshakeEntradasalida;

	handshakeEntradasalida = cfg_entradaSalida->TIPO_INTERFAZ_INT;

	send(kernel_fd, &handshakeEntradasalida, sizeof(int32_t), 0);

	pthread_t tid[2];

	pthread_create(&tid[0], NULL, recibirKernel, NULL);
	
	log_destroy(loggerIOKernel);

	return kernel_fd;
}


t_log* iniciar_logger(char *nombre)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create(nombre, "tp", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL)
	{
		printf("No se genero de forma correcta el logger");
		exit(1);
	}

	return nuevo_logger;
}


void paquete(int conexion, t_log* logger)
{	char *leido;
	t_paquete* paquete = crear_paquete(PAQUETECLIENTE,logger);

	leido = readline("> ");
	while(*leido != '\0'){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);
	free(paquete);
	free(leido);

	return;
}

void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);

	return;
}

void *recibirKernel() {

	while(1) {

		pthread_mutex_lock(&mutex_recvKernel);
		int cod_op = recibir_operacion(kernel_fd);

		switch (cod_op)
		{
		case IO_STDOUT_WRITE:
			
			pthread_mutex_unlock(&mutex_recvKernel);
			break;
		
		case IO_STDIN_READ:
			
			pthread_mutex_unlock(&mutex_recvKernel);
			break;
		
		case IO_FS_CREATE:

			pthread_mutex_unlock(&mutex_recvKernel);
			break;

		case IO_FS_DELETE:
	
			pthread_mutex_unlock(&mutex_recvKernel);
			break;

		case IO_FS_READ:
			
			pthread_mutex_unlock(&mutex_recvKernel);
			break;

		case IO_FS_TRUNCATE:
	
			pthread_mutex_unlock(&mutex_recvKernel);
			break;

		case IO_FS_WRITE:

			pthread_mutex_unlock(&mutex_recvKernel);
			break;

		case IO_GEN_SLEEP: 	

			pthread_t genSleep;
			pthread_create(&genSleep, NULL, (void *)solicitudIO_GEN_SLEEP, &kernel_fd);
			pthread_join(genSleep, NULL);
			pthread_mutex_unlock(&mutex_recvKernel);
			break;
		
		case -1:
			log_error(info_logger, "El cliente se desconecto");
			return NULL;
			break;
		
		default:
			log_warning(info_logger, "Operacion desconocida, cuidado: %d", cod_op);
			pthread_mutex_unlock(&mutex_recvKernel);
			break;
		}
	}

	return NULL;
}

void *solicitudIO_STDIN_READ(void* cliente_socket) {

	int conexion = *((int*) cliente_socket);

	t_peticion* peticion_io_stdin_read = malloc(sizeof(t_peticion));
	peticion_io_stdin_read->operacion = EJECUTAR_IO_STDIN_READ;
	t_list* listaEnteros = list_create();
	listaEnteros = recibirListaUint32_t(conexion);
	peticion_io_stdin_read->pid = *(uint32_t*)list_get(listaEnteros, 0); 
	peticion_io_stdin_read->direccionFisica = *(uint32_t*)list_get(listaEnteros, 1); 

	return NULL;
}

void* solicitudIO_GEN_SLEEP (void* cliente_socket) {

	int conexion = *((int*) cliente_socket);

	
	t_peticion* peticion_io_gen_sleep = malloc(sizeof(t_peticion));
	peticion_io_gen_sleep->operacion = EJECUTAR_IO_GEN_SLEEP;
	t_list* listaEnteros = list_create();

	listaEnteros = recibirListaUint32_t(conexion);

	peticion_io_gen_sleep->pid = *(uint32_t*)list_get(listaEnteros, 0);

	peticion_io_gen_sleep->unidadesDeTrabajo = *(uint32_t*)list_get(listaEnteros, 1);

	agregarPeticionAPendientes(peticion_io_gen_sleep);
	sem_post(&sem_contador_peticiones);
	return NULL;
}

void agregarPeticionAPendientes(t_peticion* peticion) {

	pthread_mutex_lock(&mutex_peticiones_pendientes);

	list_add(lista_peticiones_pendientes, peticion);

	pthread_mutex_unlock(&mutex_peticiones_pendientes);

	return;
}

void iniciarAtencionPeticiones() {
	pthread_t hilo_peticiones;
	log_trace(trace_logger, "Inicio atención de peticiones");

	pthread_create(&hilo_peticiones, NULL, (void *) atenderPeticiones, NULL);
	pthread_detach(hilo_peticiones);

}

void atenderPeticiones() {

	while(1) {
		sem_wait(&sem_contador_peticiones);

		log_trace(trace_logger, "Hay una peticion pendiente");

		t_peticion* peticion = sacoPeticionDePendientes();

		manejarPeticion(peticion);
	}

	return;
}

t_peticion* sacoPeticionDePendientes() {
	pthread_mutex_lock(&mutex_peticiones_pendientes);
	t_peticion* peticion = list_remove(lista_peticiones_pendientes, 0);
	pthread_mutex_unlock(&mutex_peticiones_pendientes);
	return peticion;
}

void manejarPeticion(t_peticion* peticion) {
	t_operacion_io codOpIO = peticion->operacion;


	switch (codOpIO)
	{
	case EJECUTAR_IO_GEN_SLEEP:
		manejarInterfazGenerica(peticion->unidadesDeTrabajo);
		logOperacion(peticion->pid, "IO_GEN_SLEEP");
		break;
	case EJECUTAR_IO_STDOUT_WRITE:
		
		break;
	case EJECUTAR_IO_STDIN_READ:
		
		break;
	case EJECUTAR_IO_FS_CREATE:
		
		break;
	case EJECUTAR_IO_FS_DELETE:
		
		break;
	case EJECUTAR_IO_FS_READ:
		
		break;
	case EJECUTAR_IO_FS_TRUNCATE:
		
		break;
	case EJECUTAR_IO_FS_WRITE:

		break;						
	
	default:
		log_error(info_logger, "Codigo de operacion desconocido: %d ", codOpIO);
		return;
		
	}

	free(peticion);

	return;
}


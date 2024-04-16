#include <comunicacion.h>

int cpuDispatch_fd;
int cpuInterrupt_fd;
int memoria_fd;
int kernel_fd;
int interfazIO_fd[4];

int recibirConexion(char *puerto) {
	
	t_log* logger;
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);
	pthread_t hilosIO[4];
	int contadorIO=0;

	
	kernel_fd = iniciar_servidor(logger,"Server Kernel",puerto);
	
	while(1)
	{
		interfazIO_fd[contadorIO] = esperar_cliente(kernel_fd);
		pthread_create(&hilosIO[contadorIO], NULL, recibirIO, contadorIO);
		contadorIO++;
	}

	return EXIT_SUCCESS;
}

int conectarModuloCPU(char *modulo){

	char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	t_log *logger;

	strcpy(charAux, modulo);
	logger= iniciar_logger(strcat(charAux,".log"));

	strcpy(charAux, modulo);

	strcpy(parametro, "IP_");
    strcat(parametro, charAux);
	ip=config_get_string_value(config,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(config,parametro);

	log_info(logger, "IP=%s\n", ip);
	log_info(logger, "PUERTO=%s\n", puerto);

	cpuDispatch_fd= crear_conexion(logger, "Conecto Kernel a CPU Dispatch",ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return cpuDispatch_fd;
}

int conectarModuloCPUInterrupt(char *modulo){
	
	char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	t_log *logger;

	strcpy(charAux, modulo);
	logger= iniciar_logger(strcat(charAux,".log"));

	strcpy(charAux, modulo);

	strcpy(parametro, "IP_");
    strcat(parametro, charAux);
	ip=config_get_string_value(config,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_INTERRUPT");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(config,parametro);

	log_info(logger, "IP=%s\n", ip);
	log_info(logger, "PUERTO=%s\n", puerto);

	cpuInterrupt_fd= crear_conexion(logger, "Conecto kernel a CPU Interrupt",ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return cpuInterrupt_fd;
}

int conectarModuloMemoria(char *modulo){

	char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	t_log *logger;

	strcpy(charAux, modulo);
	logger= iniciar_logger(strcat(charAux,".log"));

	strcpy(charAux, modulo);

	strcpy(parametro, "IP_");
    strcat(parametro, charAux);
	ip=config_get_string_value(config,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(config,parametro);

	log_info(logger, "IP=%s\n", ip);
	log_info(logger, "PUERTO=%s\n", puerto);

	memoria_fd= crear_conexion(logger, "Conecto Kernel a memoria",ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return memoria_fd;
}

void *recibirIO(int contador){
	
	int contadorIO_local= contador;
	while(1) {
		t_log* logger;
		logger=iniciar_logger("recibirIO.log");;

		int cod_op = recibir_operacion(interfazIO_fd[contadorIO_local]); //seguro se necesita un mutex
		// pthread_mutex_lock(&mutexFS);

		t_list *lista = list_create();
	    switch (cod_op) {
		// 	case ACCESO_PEDIDO_LECTURA:
		// 		realizarPedidoEscrituraFs(filesystem_fd);

		// 		break;
		// 	case ACCESO_PEDIDO_ESCRITURA:
		// 		realizarPedidoLecturaFs(filesystem_fd);

		// 		break;
		// 	case PEDIR_SWAP:
		// 		lista = recibirListaUint32_t(filesystem_fd);
		// 		cargarPaginasEnTabla(pidGlobal, sizeGlobal, lista);
		// 		list_destroy_and_destroy_elements(lista, free); //LINEA AGREGADA
		// 		break;
		// 	case LECTURA_BLOQUE_SWAP:
		// 		t_datos* unosDatos = malloc(sizeof(t_datos));
		// 		void* datos;

		// 		t_list* listaConSwap = recibirListaIntsYDatos(filesystem_fd,unosDatos);
		// 		uint32_t posSwap = *(uint32_t *)list_get(listaConSwap, 0);
		// 		datos = unosDatos->datos;
		// 		recibirDatosDeFs(datos, posSwap);

		// 		free(unosDatos->datos);
		// 		free(unosDatos);
		// 		list_destroy_and_destroy_elements(listaConSwap, free); //LINEA AGREGADA
		// 		break;
			 case -1:
				 log_error(logger, "el cliente se desconecto.");

				 log_error(logger, "Terminando servidor.FILESYSTEM");
				 return NULL;

			 default:

				log_warning(logger,"Operacion desconocida. No quieras meter la pata %d ", cod_op);
				break;
		}
		//pthread_mutex_unlock(&mutexFS);
	}
}

t_log* iniciar_logger(char *nombre){
	t_log* nuevo_logger;
	nuevo_logger = log_create(nombre, "tp", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		printf("No se genero de forma correcta el logger");
		exit(1);
	}

	return nuevo_logger;
}

void leer_consola(t_log* logger){
	char* leido;

	leido = readline("> ");

	while(*leido != '\0'){
		log_info(logger, "%s\n", leido);
		free(leido);
		leido = readline("> ");
	}

	free(leido);
}

void paquete(t_log* logger, char* parametro){
	t_paquete* paquete = crear_paquete(PAQUETECLIENTE,logger);

   agregar_a_paquete(paquete, parametro, strlen(parametro)+1);

	//enviar_paquete(paquete, kernel_fd);
	free(paquete);
}

void terminar_programa(int conexion, t_log* logger)
{	log_destroy(logger);
	liberar_conexion(conexion);
}
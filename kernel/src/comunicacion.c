#include <comunicacion.h>

int cpuDispatch_fd;
int cpuInterrupt_fd;
int memoria_fd;
int kernel_fd;
int vectorIO[4];


int recibirConexion(char *puerto) {

	int entradasalida_fd;
	t_log* logger;
	pthread_t tid[4];
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);
	int32_t tipoInterfaz;
	
	kernel_fd = iniciar_servidor(logger,"Server Kernel",puerto);

	while(1){
		entradasalida_fd = esperar_cliente(kernel_fd);
		recv(entradasalida_fd, &tipoInterfaz, sizeof(int32_t), MSG_WAITALL);
		vectorIO[tipoInterfaz] = entradasalida_fd;
		cualInterfaz(tipoInterfaz);
		pthread_create(&tid[tipoInterfaz], NULL, recibirIO, vectorIO[tipoInterfaz]);


		
		//enviarValor_uint32(8, vectorIO[tipoInterfaz], IO_GEN_SLEEP, logger);
		


	}


	return EXIT_SUCCESS;
}


void cualInterfaz(int tipoInterfaz){

	t_log* logger;
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);

	switch (tipoInterfaz)
	{
	case 0: //STDOUT
		log_info(logger, "Interfaz STDOUT conectada");
		break;
	case 1: //STDIN
		log_info(logger, "Interfaz STDIN conectada");
		break;
	case 2: //DIAL_FS
		log_info(logger, "Interfaz DIAL_FS conectada");
		break;
	case 3: //GENERICA
		log_info(logger, "Interfaz GENERICA conectada");
		break;
	default:
		break;
	}

	log_destroy(logger);
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

void *recibirIO(int interfaz_fd){
	

	while(1) {
		t_log* logger;
		logger=iniciar_logger("recibirIO.log");

		int cod_op = recibir_operacion(interfaz_fd); //seguro se necesita un mutex
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

				 log_error(logger, "Terminando servidor ENTRADASALIDA");
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
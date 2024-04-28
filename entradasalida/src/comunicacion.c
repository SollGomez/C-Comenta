#include <comunicacion.h>

int contadorDispositivosIO=0;
int memoria_fd;
int kernel_fd;

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
}

void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}

void *recibirKernel() {
	while(1) {
		int cod_op = recibir_operacion(kernel_fd);

		switch (cod_op)
		{
		case 0: //IO_GEN_SLEEP
			
			ejecutarIO_GEN_SLEEP(kernel_fd);

			break;
		
		default:
			break;
		}
	}
}


void ejecutarIO_GEN_SLEEP(int cliente_socket) {

	int conexion = cliente_socket;

	uint32_t unidadesDeTrabajo = recibirValor_uint32(conexion);

	manejarInterfazGenerica(unidadesDeTrabajo);

}
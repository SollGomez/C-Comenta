#include <comunicacion.h>

int contadorDispositivosIO = 0;
int memoria_fd;
int kernel_fd;

t_list* listaDeArchivos;

void* iniciarMemoria () {
	conectarMemoria("MEMORIA");
	return NULL;
}

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


	pthread_t tid;

	pthread_create(&tid, NULL, recibirMemoria, NULL);
	pthread_join(tid, NULL);

	log_destroy(loggerIOMem);

	return memoria_fd;
}

void* iniciarKernel () {
	conectarKernel("KERNEL");
	return NULL;
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

	list_create(listaDeArchivos);
	int32_t tamanioLista;

	if(strcmp(cfg_entradaSalida->TIPO_INTERFAZ, "poner dialfs") == 0){		// HABLAR CON AXO
		recv(kernel_fd, &tamanioLista, sizeof(int32_t), MSG_WAITALL);				//sizeof(lista)
		recv(kernel_fd, listaDeArchivos, sizeof(tamanioLista), MSG_WAITALL);			//recibo lista de archivos
	}

	pthread_t tid;

	pthread_create(&tid, NULL, recibirKernel, NULL);
	pthread_join(tid, NULL);
	
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

void *recibirMemoria() {

	while(1) {
		int cod_op = recibir_operacion(memoria_fd);

		switch (cod_op)
		{
		case LECTURA_REALIZADA:
			devolucionIO_STDOUT_WRITE(&memoria_fd);
			break;
			
		case ESCRITURA_REALIZADA:
			enviarOrden(SOLICITUD_IO_CUMPLIDA, kernel_fd, info_logger);
			break;

		case -1:
			log_error(info_logger, "El cliente se desconecto");
			return NULL;
			break;
		
		default:
			log_warning(info_logger, "Operacion desconocida, cuidado: %d", cod_op);
			break;
		
		}
	}
}

void *recibirKernel() {


	switch (cfg_entradaSalida->TIPO_INTERFAZ_INT)
    {
		case 0:  					//STDOUT
			while(1) {

			int cod_op = recibir_operacion(kernel_fd);

			switch (cod_op)
			{
			case IO_STDOUT_WRITE:
				solicitudIO_STDOUT_WRITE(&kernel_fd);
				break;

			case -1:
				log_error(info_logger, "El cliente se desconecto");
				return NULL;
				break;
			
			default:
				log_warning(info_logger, "Operacion desconocida, cuidado: %d", cod_op);
				break;
			}
		}
			break;
		case 1:  						//STDIN
			while(1) {

			int cod_op = recibir_operacion(kernel_fd);

			switch (cod_op)
			{
			case IO_STDIN_READ:
				solicitudIO_STDIN_READ(&kernel_fd);
				break;
			
			case -1:
				log_error(info_logger, "El cliente se desconecto");
				return NULL;
				break;
			
			default:
				log_warning(info_logger, "Operacion desconocida, cuidado: %d", cod_op);
				break;
			}
		}
			break;
		case 2: 					 	//DIALFS
			while(1) {

			int cod_op = recibir_operacion(kernel_fd);

			switch (cod_op)
			{
			case IO_FS_CREATE:

				break;

			case IO_FS_DELETE:
		
				break;

			case IO_FS_READ:
				
				break;

			case IO_FS_TRUNCATE:
		
				break;

			case IO_FS_WRITE:

				break;
			
			case -1:
				log_error(info_logger, "El cliente se desconecto");
				return NULL;
				break;
			
			default:
				log_warning(info_logger, "Operacion desconocida, cuidado: %d", cod_op);
				break;
			}
		}
			break;
		case 3:  						//GENERICA
			while(1) {

					int cod_op = recibir_operacion(kernel_fd);

				switch (cod_op)
				{
				case IO_GEN_SLEEP: 	
					solicitudIO_GEN_SLEEP(&kernel_fd);
					break;
				
				case -1:
					log_error(info_logger, "El cliente se desconecto");
					return NULL;
					break;
				
				default:
					log_warning(info_logger, "Operacion desconocida, cuidado: %d", cod_op);
						break;
				}
		}
			break;
		default:
			printf("Esa interfaz no existe :/");
			break;
    }


	return NULL;
}

void* devolucionIO_STDOUT_WRITE(void* cliente_socket) {  //Esta funcion puede causar problemas. Estar al tanto.

	int conexion = *((int*) cliente_socket);
	char* textoAMostrar = malloc(sizeof(char*));

	t_datos* datitos = malloc(sizeof(t_datos));

	t_list* listaInts;

	list_create(listaInts);

	listaInts = recibirListaIntsYDatos(conexion, datitos);

	uint32_t pid = list_get(listaInts, 0);

	usleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO * 10000);

	printf("\n\n PID <%d> - <%s>\n\n", pid, datitos->datos);

	enviarOrden(SOLICITUD_IO_CUMPLIDA, kernel_fd, info_logger);
	return NULL;
}


void* solicitudIO_STDOUT_WRITE(void* cliente_socket) {
	
	int conexion = *((int*) cliente_socket);

	t_list* listaEnteros = list_create();
	listaEnteros = recibirListaUint32_t(conexion); // 0 pid, 1 dirfisica, 2 tamanio
	uint32_t pid = *(uint32_t*)list_get(listaEnteros, 0);
	enviarListaUint32_t(listaEnteros, memoria_fd, info_logger, ACCESO_PEDIDO_LECTURA);

	log_info(info_logger, "PID: <%d> Direccion fisica enviada a memoria", pid);
	logOperacion(pid, "IO_STDOUT_WRITE");

	return NULL;
}

void *solicitudIO_STDIN_READ(void* cliente_socket) {

	int conexion = *((int*) cliente_socket);

	t_list* listaEnteros = list_create();
	listaEnteros = recibirListaUint32_t(conexion);	// 0 pid, 1 dirfisica, 2 tamanio
	uint32_t pid = *(uint32_t*)list_get(listaEnteros, 0); 
	uint32_t direccionFisica = *(uint32_t*)list_get(listaEnteros, 1);

	logOperacion(pid, "IO_STDIN_READ");
	manejarInterfazStdin(direccionFisica, pid);

	return NULL;
}

void* solicitudIO_GEN_SLEEP (void* cliente_socket) {

	int conexion = *((int*) cliente_socket);
	
	t_list* listaEnteros = list_create();
	listaEnteros = recibirListaUint32_t(conexion);

	uint32_t pid = *(uint32_t*)list_get(listaEnteros, 0);
	uint32_t unidadesDeTrabajo = *(uint32_t*)list_get(listaEnteros, 1);

	logOperacion(pid, "IO_GEN_SLEEP");
	manejarInterfazGenerica(unidadesDeTrabajo);

	enviarValor_uint32(pid, kernel_fd, SOLICITUD_IO_CUMPLIDA, info_logger);

	return NULL;
}

void* solicitudIO_FS_CREATE (void* cliente_socket) {

	int conexion = *((int*) cliente_socket);
	char* nombreArch = malloc(10);
	uint32_t pid;
	strcpy(nombreArch, recibirEnteroYString(conexion, &pid));

	crearArchivo(nombreArch);
	logCrearArchivo(pid, nombreArch);

	return NULL;
}

void* solicitudIO_FS_DELETE (void* cliente_socket) {
	int conexion = *((int*) cliente_socket);
	char* nombreArch = malloc(10);
	uint32_t pid;
	strcpy(nombreArch, recibirEnteroYString(conexion, &pid));

	eliminarArchivo(nombreArch);
	logEliminarArchivo(pid, nombreArch);
	
	return NULL;
}

void* solicitudIO_FS_TRUNCATE (void* cliente_socket) {

	//int conexion = *((int*) cliente_socket);
	return NULL;
}

void* solicitudIO_FS_WRITE (void* cliente_socket) {

	//int conexion = *((int*) cliente_socket);
	return NULL;
}

void* solicitudIO_FS_READ (void* cliente_socket) {
	
	//int conexion = *((int*) cliente_socket);
	return NULL;
}
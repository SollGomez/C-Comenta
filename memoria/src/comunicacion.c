#include <comunicacion.h>
#include "memoriaInstrucciones.h"

int memoria_fd;
int cpu_fd;
int interfazIO_fd[4];
int kernel_fd;
t_log* logger;

uint32_t pidGlobal;
uint32_t sizeGlobal;

int recibirConexion(char *puerto) {
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);
	pthread_t tid[5];
//	pthread_t hilosIO[4];
//	int contadorIO=0;

	memoria_fd = iniciar_servidor(logger, "Server Memoria" , puerto);
	log_info(logger, "Servidor listo para recibir a los clientes");

	cpu_fd = esperar_cliente(memoria_fd);
	pthread_create(&tid[3], NULL, recibirCPU, NULL);
	kernel_fd = esperar_cliente(memoria_fd);
	pthread_create(&tid[4], NULL, recibirKernel, NULL);
	int32_t tipoInterfaz;
	int entradasalida_fd;
	while (1) {
		entradasalida_fd = esperar_cliente(memoria_fd);
		recv(entradasalida_fd, &tipoInterfaz, sizeof(int32_t), MSG_WAITALL);
		interfazIO_fd[tipoInterfaz] = entradasalida_fd;
		cualInterfaz(tipoInterfaz);
		pthread_create(&tid[tipoInterfaz], NULL, recibirIO, interfazIO_fd[tipoInterfaz]);
	}
	//pthread_join(tid[0], NULL);
	//pthread_join(tid[1], NULL);
	// pthread_join(tid[2], NULL);

	return EXIT_SUCCESS;
}


void cualInterfaz(int tipoInterfaz){

	t_log* logger;
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);

	switch (tipoInterfaz)
	{
	case 0: //STDOUT
		log_info(logger, "Interfaz STDOUT conectada");
		recibirIO(tipoInterfaz);
		break;
	case 1: //STDIN
		log_info(logger, "Interfaz STDIN conectada");
		recibirIO(tipoInterfaz);
		break;
	case 2: //DIAL_FS
		log_info(logger, "Interfaz DIAL_FS conectada");
		recibirIO(tipoInterfaz);
		break;
	default:
		break;
	}

	log_destroy(logger);
}

 void *recibirCPU(void) {
	while(1) {
		int cod_op = recibir_operacion(cpu_fd);
		t_list *lista;
		switch (cod_op) {
			case HANDSHAKE_CPU:
				recibirOrden(cpu_fd);
				log_info(logger,"HANDSHAKE con CPU acontecido");
				PaqueteHand(cpu_fd, logger);
				break;
			case SOLICITUDMARCO:
				lista = recibirListaUint32_t(cpu_fd);
				uint32_t marco = obtenerMarcoDePagina(*(uint32_t*)list_get(lista,0), *(uint32_t*)list_get(lista,1));
				enviarValor_uint32(marco, cpu_fd, SOLICITUDMARCO, info_logger);
//					list_clean(lista);
				// list_destroy_and_destroy_elements(lista, free); //LINEA MODIFICADA
				break;
				
			case ACCESO_PEDIDO_LECTURA:
				realizarPedidoLectura(cpu_fd);
				break;

			case ACCESO_PEDIDO_ESCRITURA:
				realizarPedidoEscritura(cpu_fd);
				break;

			case RESIZE:
				lista = recibirListaUint32_t(cpu_fd);
				uint32_t resultado = resizeProceso(*(uint32_t*)list_get(lista,0), *(uint32_t*)list_get(lista,1)); //pid tamanio
				enviarValor_uint32(resultado, cpu_fd, RESIZE, info_logger);

				break;

			case SOLICITUDINSTRUCCION:
				lista = recibirListaUint32_t(cpu_fd);
				t_paquete* paquete = crear_paquete(SOLICITUDINSTRUCCION, info_logger);
				Instruccion* instruccion;
				log_trace(trace_logger, "PID: %d PC: %d", *(uint32_t*)list_get(lista,0),*(uint32_t*)list_get(lista,1));
				instruccion = retornarInstruccionACPU(*(uint32_t*)list_get(lista,0),*(uint32_t*)list_get(lista,1)); // pid y pc
				//log_trace(trace_logger, "Instruccion: %s %s %s", instruccion->id, instruccion->param1, instruccion->param2);
				usleep(RETARDO_RESPUESTA*1000); //ver si cambiar a sleep
				// log_info(info_logger, "instruccion: %s %s %s %s %s %s\n", instruccion->id, instruccion->param1, instruccion->param2
				// 														, instruccion->param3, instruccion->param4, instruccion->param5);
				agregar_instruccion_a_paquete(paquete, instruccion);
				enviar_paquete(paquete, cpu_fd);
				eliminar_paquete(paquete);
//					list_clean(lista);
//					list_destroy_and_destroy_elements(lista, free); // ESTO ERA SOLO LIST_DESTRO
				break;
			case -1:
				log_error(logger, "el cliente se desconecto.");
				log_error(logger, "Terminando servidor.CPU");
				return NULL;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}

void *recibirIO(int interfaz_fd) {
  	while(1) {
  		int cod_op = recibir_operacion(interfaz_fd); //seguro se necesita un mutex;
  		switch (cod_op) {
 			case IO_STDIN_READ_DONE:{
				realizarPedidoEscritura(interfaz_fd);
 				break;
			} //ACCESO_PEDIDO_ESCRITURA
 			case IO_STDOUT_WRITE_LEER_DIRECCION_EN_MEMORIA:{
				realizarPedidoLectura(interfaz_fd);
 				break;
			} //ACCESO_PEDIDO_LECTURA
  			case -1:{
  				log_error(logger, "el cliente se desconecto.");
  				log_error(logger, "Terminando servidor. I/O");
  				return NULL;
			}
  			default:{
  				log_warning(logger,"Operacion desconocida. No quieras meter la pata %d ", cod_op);
  				break;
			}
  		}
  	}
}


void *recibirKernel() {
	while(1) {
		int cod_op = recibir_operacion(kernel_fd);
 		switch (cod_op) {
        	case INICIALIZAR_PROCESO_MEMORIA: 
        		inicializarProceso(kernel_fd);
        		break;

        	case FINALIZAR_PROCESO_MEMORIA:
        		finalizarProceso(kernel_fd);
        		break;

 			case -1:
 				log_error(logger, "el cliente se desconecto.");
 				log_error(logger, "Terminando servidor KERNEL");
 				return NULL;
				
 			default:
 				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
 				break;
 		}
 	}
 }

t_log* iniciar_logger(char *nombre) {
	t_log* nuevo_logger;
	nuevo_logger = log_create(nombre, "tp", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		printf("No se genero de forma correcta el logger");
		exit(1);
	}
	return nuevo_logger;
}

void PaqueteHand(int conexion, t_log* logger){
	t_paquete* paquete = crear_paquete(HANDSHAKE_CPU,logger);

	char buffer[20];
	sprintf(buffer, "%d", TAM_PAGINA);

	agregar_a_paquete(paquete, buffer, strlen(buffer)+1);

	enviar_paquete(paquete, conexion);
	free(paquete);
}

void realizarPedidoLectura(int cliente_socket){			//Vale para io y cpu. Les manda LECTURA_REALIZADA
    t_list* listaInts = recibirListaUint32_t(cliente_socket);	// 0 pid, 1 dirfisica, 2 tamanio
    uint32_t pid = *(uint32_t*)list_get(listaInts,0);
    uint32_t posicion = *(uint32_t*)list_get(listaInts,1);
    uint32_t tamanio = *(uint32_t*)list_get(listaInts,2);

    pthread_mutex_lock(&mutex_espacioContiguo);
    log_trace(trace_logger,"Accediendo a Espacio de Usuario para Lectura en la Dirección: <%d> de Tamanio: <%d> para el Proceso con PID: <%d>", posicion, tamanio, pid);
    uint32_t datos = manejarLectura(posicion, tamanio, pid);
    log_trace(trace_logger,"Se accedió a Espacio de Usuario correctamente");
    pthread_mutex_unlock(&mutex_espacioContiguo);
	list_add(listaInts, &datos);

	enviarListaUint32_t(listaInts, cliente_socket, info_logger, LECTURA_REALIZADA);
}

uint32_t manejarLectura(uint32_t posInicial, uint32_t tamanio, uint32_t pid) {
	uint32_t datos = 0;
	void* datosPuntero = &datos;
	uint32_t lei = 0;
	uint32_t frameQueCorresponde = posInicial / TAM_PAGINA;
	uint32_t tamPrimerFrame = TAM_PAGINA * (frameQueCorresponde + 1) - posInicial; //cuanto puedo leer en el frame actual
	uint32_t pagActual;

	while(tamanio > tamPrimerFrame) {
		memcpy(datosPuntero+lei, leerMemoria(posInicial, tamPrimerFrame, pid), tamPrimerFrame);
		lei += tamPrimerFrame;
		pagActual = obtenerPaginaConMarco(frameQueCorresponde);
		frameQueCorresponde = obtenerMarcoDePagina(pid, pagActual+1);
		posInicial = frameQueCorresponde * TAM_PAGINA;
		tamanio -= tamPrimerFrame;
		tamPrimerFrame = TAM_PAGINA * (frameQueCorresponde + 1) - posInicial;
	}
	memcpy(datosPuntero+lei, leerMemoria(posInicial, tamanio, pid), tamanio);
	return datos;
}

void manejarEscritura(uint32_t posInicial, uint32_t datos, uint32_t pid, uint32_t tamanio) {

	void* punteroDatos = &datos;
	uint32_t escribi = 0;
	uint32_t frameQueCorresponde = posInicial / TAM_PAGINA;
	uint32_t tamPrimerFrame = TAM_PAGINA * (frameQueCorresponde + 1) - posInicial; //cuanto puedo leer en el frame actual
	uint32_t pagActual;

	while(tamanio > tamPrimerFrame) {
		log_trace(trace_logger, "tamanio a leer %d, tamanio que puedo leer en esta pag %d", tamanio, tamPrimerFrame);
		escribirMemoria(posInicial, punteroDatos+escribi, tamPrimerFrame, pid);
		escribi += tamPrimerFrame;
		pagActual = obtenerPaginaConMarco(frameQueCorresponde);
		frameQueCorresponde = obtenerMarcoDePagina(pid, pagActual+1);
		posInicial = frameQueCorresponde * TAM_PAGINA;
		tamanio = tamanio - tamPrimerFrame;
		tamPrimerFrame = TAM_PAGINA * (frameQueCorresponde + 1) - posInicial;
		log_trace(trace_logger, "escribo en la siguiente pagina");
	}
	escribirMemoria(posInicial, punteroDatos+escribi, tamanio, pid);
	return;
}

void realizarPedidoEscritura(int cliente_socket){		//Vale para io y cpu. Les manda ESCRITURA_REALIZADA

	t_list* listaInts = recibirListaUint32_t(cliente_socket);
    uint32_t* pid = list_get(listaInts, 0);
    uint32_t* posicion = list_get(listaInts, 1);
    uint32_t* tamanio = list_get(listaInts, 2);
	uint32_t* valor = list_get(listaInts, 3);

    pthread_mutex_lock(&mutex_espacioContiguo);
    log_trace(trace_logger,"me llego pid %d, pos %d y tamanio %d y valor %d", *pid, *posicion, *tamanio, *valor);
	manejarEscritura(*posicion, *valor, *pid, *tamanio);

    list_clean_and_destroy_elements(listaInts, free);
    list_destroy(listaInts);
    pthread_mutex_unlock(&mutex_espacioContiguo);
    enviarOrden(ESCRITURA_REALIZADA, cliente_socket, info_logger);
}

void inicializarProceso(int cliente_socket){

    char* file_name = recibirEnteroEnteroChar(cliente_socket, &pidGlobal, &sizeGlobal); 
    log_trace(trace_logger, "%s %d %d", file_name, pidGlobal, sizeGlobal);

	GuardarInstrucsDeProceso(pidGlobal, file_name);

    crearTablaPaginasProceso(pidGlobal, sizeGlobal);
	sleep(1);
	log_trace(trace_logger,"Tamaño de tablaGeneral antes de terminar inicializarProceso: %d", list_size(tablaGeneral));

	enviarOrden(INICIALIZAR_PROCESO_MEMORIA,cliente_socket, trace_logger);
}

void finalizarProceso(int cliente_socket){
	log_trace(trace_logger,"Tamaño de tabla General al llegar a finalizar Proceso: %d", list_size(tablaGeneral));
    pthread_mutex_lock(&mutex_espacioContiguo);
    uint32_t pid = recibirValor_uint32(cliente_socket);
	TablaDePaginas* tabla = obtenerTablaPorPID(pid);
	if (tabla!=NULL){
		for (int j = 0; j < list_size(tabla->paginas); j++) {
			Pagina* pagina = list_get(tabla->paginas, j);
			marcarMarcoLibre(pagina->marco);
		}
	}

	bool buscarPorPID(void* programa){
		return ((Proceso*)programa)->pid == pid;
	}

	Proceso* programa = list_find(instruccionesDeProcesos, buscarPorPID);
	for(int i=0;i<list_size(programa->instrucciones);i++){
		Instruccion* instruccion = list_get(programa->instrucciones,i);
		free(instruccion);
	}

	list_remove_by_condition(instruccionesDeProcesos, buscarPorPID);
	//free(programa);


    if(tabla!=NULL)
    	liberarTablaDePaginas(pid);
    pthread_mutex_unlock(&mutex_espacioContiguo);
    enviarOrden(FINALIZAR_PROCESO_MEMORIA, cliente_socket, info_logger);
    log_trace(trace_logger, "Proceso finalizado con éxito");
    log_trace(trace_logger,"Tamaño de tablaGeneral al terminar finalizarProceso: %d", list_size(tablaGeneral));
}

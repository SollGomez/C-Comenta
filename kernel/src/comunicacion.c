#include <comunicacion.h>

int cpuDispatch_fd;
int cpuInterrupt_fd;
int memoria_fd;
int kernel_fd;
int vectorIO[4];

int recibirConexion (char* puerto) {
	int entradasalida_fd;
	t_log* logger;
	pthread_t tid[4];
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);
	int32_t tipoInterfaz;
	
	kernel_fd = iniciar_servidor(logger, "Server Kernel", puerto);

	while (1) {
		entradasalida_fd = esperar_cliente(kernel_fd);
		recv(entradasalida_fd, &tipoInterfaz, sizeof(int32_t), MSG_WAITALL);
		vectorIO[tipoInterfaz] = entradasalida_fd;
		cualInterfaz(tipoInterfaz);
		pthread_create(&tid[tipoInterfaz], NULL, recibirIO, vectorIO[tipoInterfaz]);

		uint32_t pid = 10;
		uint32_t unidadesTrabajo = 8;
		t_list* listaDatosDePrueba = list_create();
		list_add(listaDatosDePrueba, &pid);
		list_add(listaDatosDePrueba, &unidadesTrabajo);
		//enviarValor_uint32(8, vectorIO[tipoInterfaz], IO_GEN_SLEEP, logger);
		enviarListaUint32_t(listaDatosDePrueba, vectorIO[tipoInterfaz], logger, IO_STDIN_READ);


	}

	return EXIT_SUCCESS;
}


void cualInterfaz (int tipoInterfaz) {
	t_log* logger;
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);

	switch (tipoInterfaz){
	case 0:
		log_info(logger, "Interfaz STDOUT conectada");
		break;
	case 1:
		log_info(logger, "Interfaz STDIN conectada");
		break;
	case 2:
		log_info(logger, "Interfaz DIAL_FS conectada");
		break;
	case 3:
		log_info(logger, "Interfaz GENERICA conectada");
		break;
	default:
		break;
	}

	log_destroy(logger);
}

int conectarModuloCPU(char* modulo){
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

	return memoria_fd;
}

void* recibirIO (int interfaz_fd) {
	while (1) {
		t_log* logger;
		logger = iniciar_logger("recibirIO.log");

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

PCB* obtenerPcbExec(){
    pthread_mutex_lock(&mutex_colaExec);
    PCB* unaPcb = list_get(colaExec,0);
    pthread_mutex_unlock(&mutex_colaExec);
    return unaPcb;
}

void moverProceso_BloqrecursoReady(Recurso* recurso){
    pthread_mutex_lock(&semaforos_io[recurso->indiceSemaforo]);
    PCB* pcbLiberada = list_remove(recurso->cola,0);
    pthread_mutex_unlock(&semaforos_io[recurso->indiceSemaforo]);
    uint32_t num;
    PCB* pcbEncontrado = encontrarProceso(pcbLiberada->id, &num);
    if(pcbEncontrado != NULL){

		pthread_mutex_lock(&mutex_colaBloq);
		eliminarElementoLista(pcbLiberada, colaBloq);
		pthread_mutex_unlock(&mutex_colaBloq);

		pthread_mutex_lock(&mutex_ColaReady);
		list_add(colaReady,pcbLiberada);
		pthread_mutex_unlock(&mutex_ColaReady);
		sem_post(&sem_procesosReady);
		log_info(info_logger,"PID: <%d> - Estado Anterior: <BLOCKED_RECURSO[%s]> - Estado Actual: <READY>",pcbLiberada->id,recurso->nombreRecurso);
    }
}

void escucharCPU (void) {
	while (cpuDispatch_fd != -1)	{
		int cod_op = recibir_operacion(cpuDispatch_fd);

		switch(cod_op){
		    case WAIT: {
				PCB* pcbRecibida = recibir_contextoEjecucion_y_char(cpuDispatch_fd);
				char* nombreArchivo = pcbRecibida->nombreRecurso;
				actualizarPcbEjecucion(pcbRecibida);
				manejoDeRecursos("WAIT", nombreArchivo);
				break;
			}

			case SIGNAL: {
				PCB* pcbRecibida = recibir_contextoEjecucion_y_char(cpuDispatch_fd);
				char* nombreArchivo=pcbRecibida->nombreRecurso;
				actualizarPcbEjecucion(pcbRecibida);
				manejoDeRecursos("SIGNAL", nombreArchivo);
				break;
			}

			case INTERRUPCIONCPU: {
				PCB* pcbRecibida = recibir_contextoEjecucion(cpuDispatch_fd);
				if (list_size(colaExec)) {
					actualizarPcbEjecucion(pcbRecibida);
					PCB* pcbActualizada = obtenerPcbExec();
					moverProceso_ExecReady(pcbActualizada);
				}
				break;
			}

			case EXIT: {
				PCB* pcbRecibida = recibir_contextoEjecucion(cpuDispatch_fd);
				actualizarPcbEjecucion(pcbRecibida);
				PCB* pcbActualizada = obtenerPcbExec();
				log_info(info_logger,"Finaliza el proceso <%d> - Motivo: <SUCCESS>",pcbActualizada->id);
				moverProceso_ExecExit(pcbActualizada);
				break;
			}

			case IO_GEN_SLEEP_OPC: {
				uint32_t tiempoSleep;
				uint32_t interfaz;
				PCB* pcbRecibida = recibir_contextoEjecucion_y_uint32_y_uint32(cpuDispatch_fd, interfaz, tiempoSleep);
				actualizarPcbEjecucion(pcbRecibida);
				PCB* pcbActualizada = obtenerPcbExec();
				t_paquete* paquete = crear_paquete(IO_GEN_SLEEP_OPC, info_logger);
				agregar_a_paquete2(paquete, &tiempoSleep, sizeof(uint32_t));
				enviar_paquete(paquete, vectorIO[interfaz]);//recibir con recibirValor_uint32
				eliminar_paquete(paquete);

				break;
			}
			case -1:
				log_error(info_logger, "Cliente desconectado de %s...", "CPU");
				return;
			default:
				log_error(info_logger, "Algo anduvo mal en el server de %s", "CPU");
				log_info(info_logger, "Cop: %d", cod_op);
				return;
		}
	}
}

void actualizarPcbEjecucion(PCB* pcbRecibida){
    pthread_mutex_lock(&mutex_colaExec);
    PCB* pcbExec = list_get(colaExec, 0);

    RegistrosCPU* registrosAux = pcbExec->registros;
    pcbExec->program_counter = pcbRecibida->program_counter;
    pcbExec->registros = pcbRecibida->registros;
    pcbRecibida->registros = registrosAux;
    liberarPcbCpu(pcbRecibida);
    pthread_mutex_unlock(&mutex_colaExec);
}

void manejoDeRecursos(char* orden, char* recursoSolicitado){
    PCB* unaPcb = obtenerPcbExec();

    bool coincideConSolicitado (Recurso* unRecurso) {
        return strcmp(unRecurso->nombreRecurso, recursoSolicitado) == 0;
    }

    Recurso* recursoEncontrado = list_find(estadoBlockRecursos, coincideConSolicitado);
    if (recursoEncontrado == NULL) {
        log_info(info_logger,"Recurso <%s> solicitado INEXISTENTE", recursoSolicitado);
        log_info(info_logger,"Finaliza el proceso <%d> - Motivo: <INVALID_RESOURCE>",unaPcb->id);
        moverProceso_ExecExit(unaPcb);
    } else {
        if (strcmp(orden,"WAIT") == 0) {
            waitRecursoPcb(recursoEncontrado, unaPcb);
        } else {
            signalRecursoPcb(recursoEncontrado, unaPcb);
        }
    }
    free(recursoSolicitado);
}

void waitRecursoPcb (Recurso* recurso, PCB* unaPcb) {
    recurso->instanciasRecurso--;
    list_add(unaPcb->recursosTomados,recurso);
    log_info(info_logger,"PID: <%d> - Wait: <%s> - Instancias: <%d>", unaPcb->id, recurso->nombreRecurso, recurso->instanciasRecurso);
    if (recurso->instanciasRecurso < 0) {
        log_info(info_logger,"PID: <%d> - Bloqueado por: <%s>",unaPcb->id,recurso->nombreRecurso);
        bloquearProcesoPorRecurso(recurso);
    } else {
    	t_paquete* paquete= crear_paquete(CONTEXTOEJECUCION, info_logger);
		agregar_ContextoEjecucion_a_paquete(paquete, unaPcb);
		enviar_paquete(paquete, cpuDispatch_fd);
		eliminar_paquete(paquete);
    }
}

void signalRecursoPcb (Recurso* recurso, PCB* unaPcb) {
    recurso->instanciasRecurso++;
    list_remove_element(unaPcb->recursosTomados,recurso);
    log_info(info_logger,"PID: <%d> - Signal: <%s> - Instancias: <%d>", unaPcb->id, recurso->nombreRecurso, recurso->instanciasRecurso);
    if (!list_is_empty(recurso->cola))
        moverProceso_BloqrecursoReady(recurso);
    t_paquete* paquete = crear_paquete(CONTEXTOEJECUCION, info_logger);
	agregar_ContextoEjecucion_a_paquete(paquete, unaPcb);
	enviar_paquete(paquete, cpuDispatch_fd);
	eliminar_paquete(paquete);
}

void bloquearProcesoPorRecurso(Recurso* recurso){
    pthread_mutex_lock(&mutex_colaExec);
    PCB* pcbABlockedRecurso = list_remove(colaExec,0);
    pthread_mutex_unlock(&mutex_colaExec);

    sem_post(&sem_cpuLibre);

    pthread_mutex_lock(&mutex_colaBloq);
	list_add(colaBloq,pcbABlockedRecurso);
	pthread_mutex_unlock(&mutex_colaBloq);

	pthread_mutex_lock(&semaforos_io[recurso->indiceSemaforo]);
	list_add(recurso->cola, pcbABlockedRecurso);
	pthread_mutex_unlock(&semaforos_io[recurso->indiceSemaforo]);

    log_info(info_logger,"PID: <%d> - Estado Anterior: <EXEC> - Estado Actual: <BLOCKED_RECURSO[%s]>", pcbABlockedRecurso->id, recurso->nombreRecurso);
}
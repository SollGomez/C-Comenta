#include <comunicacion.h>

int memoria_fd;
int cpu_fd;
int interfazIO_fd[4];
int kernel_fd;
t_log* logger;

int recibirConexion(char *puerto) {
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);
	pthread_t tid[2];
	pthread_t hilosIO[4];
	int contadorIO=0;

	memoria_fd = iniciar_servidor(logger, "Server Memoria", puerto);
	log_info(logger, "Servidor listo para recibir a los clientes");

	cpu_fd = esperar_cliente(memoria_fd);
	pthread_create(&tid[0], NULL, recibirCPU, NULL);
	kernel_fd = esperar_cliente(memoria_fd);
	pthread_create(&tid[1], NULL, recibirKernel, NULL);
	int32_t tipoInterfaz;
	int entradasalida_fd;
	while(1){
		entradasalida_fd = esperar_cliente(memoria_fd);
		recv(entradasalida_fd, &tipoInterfaz, sizeof(int32_t), MSG_WAITALL);
		interfazIO_fd[tipoInterfaz] = entradasalida_fd;
		cualInterfaz(tipoInterfaz);
		pthread_create(&tid[tipoInterfaz], NULL, recibirIO, interfazIO_fd[tipoInterfaz]);
	}
	// 
	// 
	

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




void *recibirCPU(void){
		while(1) {
			int cod_op = recibir_operacion(cpu_fd);
			t_list *lista;
			switch (cod_op) {
// 				case HANDSHAKE_CPU:
// 					recibirOrden(cpu_fd);
// 					log_info(logger,"HANDSHAKE con CPU acontecido");
// 					PaqueteHand(cpu_fd, logger);
// 					break;
// 				case SOLICITUDMARCO:
// 					lista = recibirListaUint32_t(cpu_fd);
// 					uint32_t marco = obtenerMarcoDePagina(*(uint32_t*)list_get(lista,0), *(uint32_t*)list_get(lista,1));
// 					enviarValor_uint32(marco, cpu_fd, SOLICITUDMARCO, info_logger);
// //					list_clean(lista);
// 					list_destroy_and_destroy_elements(lista, free); //LINEA MODIFICADA
// 					break;
// 				case ACCESO_PEDIDO_LECTURA:
// 					realizarPedidoLectura(cpu_fd);
// 					break;
// 				case ACCESO_PEDIDO_ESCRITURA:
// 					realizarPedidoEscritura(cpu_fd);
// 					break;

// 				case SOLICITUDINSTRUCCION:
// 					lista = recibirListaUint32_t(cpu_fd);
// 					t_paquete* paquete = crear_paquete(SOLICITUDINSTRUCCION, info_logger);
// 					Instruccion* instruccion;
// 					log_info(logger, "PID: %d PC: %d", *(uint32_t*)list_get(lista,0),*(uint32_t*)list_get(lista,1));
// 					instruccion = retornarInstruccionACPU(*(uint32_t*)list_get(lista,0),*(uint32_t*)list_get(lista,1));
// 					usleep(RETARDO_RESPUESTA*1000);
// 					log_info(info_logger, "instruccion: %s %s %s\n", instruccion->id, instruccion->param1, instruccion->param2);
// 					agregar_instruccion_a_paquete(paquete, instruccion);
// 					enviar_paquete(paquete, cpu_fd);
// 					eliminar_paquete(paquete);
// //					list_clean(lista);
// 					list_destroy_and_destroy_elements(lista, free); // ESTO ERA SOLO LIST_DESTROY
// 					break;
				case -1:
					log_error(logger, "el cliente se desconecto.");

						log_error(logger, "Terminando servidor.CPU");
						return NULL;
					break;//sacar
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
}

void *recibirIO(int interfaz_fd){

	while(1) {
		
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

				 log_error(logger, "Terminando servidor.FILESYSTEM");
				 return NULL;

			 default:

				log_warning(logger,"Operacion desconocida. No quieras meter la pata %d ", cod_op);
				break;
		}
		//pthread_mutex_unlock(&mutexFS);
	}
}

void *recibirKernel(){
	while(1) {
		int cod_op = recibir_operacion(kernel_fd);
		switch (cod_op) {
//         	case INICIALIZAR_PROCESO_MEMORIA:
//         		inicializarProceso(kernel_fd);
//         		break;
//         	case FINALIZAR_PROCESO_MEMORIA:
//         		finalizarProceso(kernel_fd);
//         		break;
//         	case CARGA_PAGINA:
//         		t_list *lista;
//         		uint32_t pid;
//         		lista = recibirListaUint32_t(kernel_fd);
//         		pid = *(uint32_t*)list_get(lista,0);
//         		TablaDePaginas* tabla;
//         		tabla = obtenerTablaPorPID(pid);
//         		cargarPaginaEnMemoria(tabla,*(uint32_t*)list_get(lista,1));
//         		sleep(2);
//         		enviarOrden(CARGA_PAGINA, kernel_fd, info_logger);
// //        		list_clean(lista);
// //        		list_destroy(lista); //LINEA AGREGADA
//         		break;
			case -1:
				log_error(logger, "el cliente se desconecto.");
				log_error(logger, "Terminando servidor KERNEL");
				return NULL;
				break;//sacar
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}


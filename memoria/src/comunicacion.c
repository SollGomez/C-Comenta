#include <comunicacion.h>

int memoria_fd;
int cpu_fd;
int interfazIO_fd;
int kernel_fd;


int recibirConexion(char *puerto) {
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_DEBUG);
	pthread_t tid[3];

	memoria_fd = iniciar_servidor(puerto);
	log_info(logger, "Servidor listo para recibir a los clientes");

	cpu_fd = esperar_cliente(memoria_fd);
	pthread_create(&tid[0], NULL, recibirCPU, NULL);
	// kernel_fd = esperar_cliente(memoria_fd);
	// pthread_create(&tid[1], NULL, recibirFS, NULL);
	// interfazIO_fd = esperar_cliente(memoria_fd);
	// pthread_create(&tid[2], NULL, recibirKernel, NULL);

	pthread_join(tid[0], NULL);
	// pthread_join(tid[1], NULL);
	// pthread_join(tid[2], NULL);

	return EXIT_SUCCESS;
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


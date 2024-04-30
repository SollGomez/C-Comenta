#include <comunicacion.h>

t_log *loggerCpuMem;

PCB* PCB_Actual;
int pid;
int kernel_fd;
int kernel_interrupt_fd;
int memoria_fd;

int conectarModulo(char *modulo){

	char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	strcpy(charAux, modulo);
	loggerCpuMem= iniciar_logger(strcat(charAux,".log"));

	strcpy(charAux, modulo);

	strcpy(parametro, "IP_");
    strcat(parametro, charAux);
	ip=config_get_string_value(config,parametro);

	if(!strcmp(charAux,"CPU"))
	 	strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(config,parametro);

	log_info(loggerCpuMem, "IP=%s\n", ip);
	log_info(loggerCpuMem, "PUERTO=%s\n", puerto);


	memoria_fd= crear_conexion(loggerCpuMem, "Conecto a Cpu a memoria",ip, puerto);

	return memoria_fd;
}

void recibirConexion(char *puerto) {
	t_log* logger;
	logger = log_create("modulo.log", "-", 1, LOG_LEVEL_INFO);

	int cpu_dispatch_fd = iniciar_servidor(logger, "Servidor CPU Dispatch", puerto);
	log_info(logger, "CPU DISPATCH: Servidor listo para recibir a los clientes");


	kernel_fd = esperar_cliente(cpu_dispatch_fd);

	pid=-1;

	while(1) {

		int cod_op = recibir_operacion(kernel_fd);

		switch (cod_op) {

		 case CONTEXTOEJECUCION: {

			PCB_Actual = recibir_contextoEjecucion(kernel_fd);

			if (PCB_Actual->id == pid) {
				ciclo_de_instruccion();
				liberarPcbCpu(PCB_Actual);

				break;
			} else {
				pid = PCB_Actual->id;
				copiar_registroPCB_a_los_registrosCPU(PCB_Actual->registros);
				ciclo_de_instruccion();
				liberarPcbCpu(PCB_Actual);

				break;
			}

		}
			case -1:
				log_error(logger, "Cliente desconectado de %s...", "SERVER CPU");
				return;
			default:
				log_error(logger, "Algo anduvo mal en el server de %s", "SERVER CPU");
				log_info(info_logger, "Cop: %d", cod_op);
				return;
		}
	}

	return;
}

void recibirConexionInterrupt(char *puerto) {    //habría q hacer un while que espere cosas de memoria
	t_log* loggerI;
	loggerI = log_create("moduloInterrupt.log", "-", 1, LOG_LEVEL_INFO);

	int cpu_interrupt_fd = iniciar_servidor(loggerI, "Servidor cpu Interrupt",puerto);
	log_info(loggerI, "CPU INTERRUPT: Servidor listo para recibir a los clientes");


	kernel_interrupt_fd = esperar_cliente(cpu_interrupt_fd);

	while(1) {
			int cod_op = recibir_operacion(kernel_interrupt_fd);
			switch (cod_op) {
				case INTERRUPCIONCPU:
					uint32_t pidInterrupt = recibirValor_uint32(kernel_interrupt_fd);
					if(pid==pidInterrupt)
						interrupciones=1;
					log_info(info_logger,"Interrupcion detectada");
					break;
				case DESALOJOCPU:
					uint32_t pidInterrupt2 = recibirValor_uint32(kernel_interrupt_fd);
					if(pid==pidInterrupt2){
						copiar_registrosCPU_a_los_registroPCB(PCB_Actual->registros);
						t_paquete* paquete = crear_paquete(EXIT, info_logger);
						agregar_ContextoEjecucion_a_paquete(paquete, PCB_Actual);
						enviar_paquete(paquete, kernel_fd);
						eliminar_paquete(paquete);
						cicloInstrucciones = false;
					}
					break;
				case -1:
					log_error(loggerI, "el cliente se desconecto.");

						log_error(loggerI, "Terminando servidor.");
						return;
					break;
				default:
					log_warning(loggerI,"Operacion desconocida. No quieras meter la pata");
					break;
			}
	}
	return;
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


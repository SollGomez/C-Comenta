#include <comunicacion.h>

int cpuDispatch_fd;
int cpuInterrupt_fd;
int memoria_fd;
int filesystem_fd;

int conectarModuloCPU(char *modulo){
	printf("ENTRE A CONECTAR ");
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

	cpuDispatch_fd= crear_conexion(ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return cpuDispatch_fd;
}

int conectarModuloCPUInterrupt(char *modulo){
	printf("ENTRE A CONECTAR ");
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

	cpuInterrupt_fd= crear_conexion(ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return cpuInterrupt_fd;
}

int conectarModuloMemoria(char *modulo){
	printf("ENTRE A CONECTAR ");
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

	memoria_fd= crear_conexion(ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return memoria_fd;
}

int conectarModuloFilesystem(char *modulo){
	printf("ENTRE A CONECTAR ");
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


	filesystem_fd= crear_conexion(ip, puerto);

	log_destroy(logger);
//	terminar_programa(conexion, logger);

	return filesystem_fd;
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

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(int conexion, t_log* logger)
{	log_destroy(logger);
	liberar_conexion(conexion);
}
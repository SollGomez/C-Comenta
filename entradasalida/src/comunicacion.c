#include "comunicacion.h"

int contadorDispositivosIO=0;
int memoria_fd[4];
int kernel_fd[4];

int conectarMemoria(char *modulo){
	printf("ENTRE A CONECTAR ");

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
	ip=config_get_string_value(config,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(config,parametro);

	log_info(loggerIOMem, "IP=%s\n", ip);
	log_info(loggerIOMem, "PUERTO=%s\n", puerto);

    log_destroy(loggerIOMem);

	memoria_fd[contadorDispositivosIO]= crear_conexion(ip, puerto);

	return memoria_fd[contadorDispositivosIO];
}

int conectarKernel(char *modulo){
	printf("ENTRE A CONECTAR ");

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
	ip=config_get_string_value(config,parametro);

	if(!strcmp(charAux,"CPU"))
		strcpy(charAux,"CPU_DISPATCH");

	strcpy(parametro, "PUERTO_");
    strcat(parametro, charAux);
	puerto=config_get_string_value(config,parametro);

	log_info(loggerIOMem, "IP=%s\n", ip);
	log_info(loggerIOMem, "PUERTO=%s\n", puerto);

    log_destroy(loggerIOMem);

	kernel_fd[contadorDispositivosIO]= crear_conexion(ip, puerto);

	return kernel_fd[contadorDispositivosIO];
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

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}


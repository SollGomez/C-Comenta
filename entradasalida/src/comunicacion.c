#include "comunicacion.h"

int contadorDispositivosIO=0;
int memoria_fd[50];
int kernel_fd[50];

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
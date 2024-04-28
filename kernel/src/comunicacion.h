#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include <utils/utilidades.h>
#include "pthread.h"
#include "main.h"


extern t_config *config;

extern int cpuDispatch_fd;
extern int cpuInterrupt_fd;
extern int memoria_fd;
extern int kernel_fd;
extern int interfazIO_fd[4];

int recibirConexion(char *puerto);
int conectarModuloCPU(char *);
int conectarModuloCPUInterrupt(char *);
int conectarModuloMemoria(char *);
void *recibirIO(int fdInterfaz);
void cualInterfaz(int tipoInterfaz);

t_log* iniciar_logger(char*);
void leer_consola(t_log*);
void paquete(t_log*, char*);
void terminar_programa(int, t_log*);
void iterator(char* value);

#endif
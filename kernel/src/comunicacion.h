#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "pthread.h"


extern t_config *config;

extern int cpuDispatch_fd;
extern int cpuInterrupt_fd;
extern int memoria_fd;
extern int filesystem_fd;

int conectarModuloCPU(char *);
int conectarModuloCPUInterrupt(char *);
int conectarModuloMemoria(char *);
int conectarModuloFilesystem(char *);

t_log* iniciar_logger(char*);
void leer_consola(t_log*);
void paquete(t_log*, char*);
void terminar_programa(int, t_log*);
int recibirConexion(char *, int);
void iterator(char* value);

#endif
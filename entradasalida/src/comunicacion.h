#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/utilidades.h>
#include "pthread.h"
#include <manejarInterfaz.h>

extern int contadorDispositivosIO;
extern int memoria_fd;
extern int kernel_fd;

int conectarKernel(char *modulo);
int conectarMemoria(char *modulo);
t_log* iniciar_logger(char*);
void paquete(int, t_log*);
void terminar_programa(int, t_log*);
void iterator(char* value);
void *recibirKernel();
void ejecutarIO_GEN_SLEEP(int cliente_socket);

#endif
#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "pthread.h"
#include "main.h"

extern int contadorDispositivosIO;
extern int memoria_fd[4];
extern int kernel_fd[4];

int conectarKernel(char *modulo);
int conectarMemoria(char *modulo);
t_log* iniciar_logger(char*);
void paquete(int, t_log*);
void terminar_programa(int, t_log*);
void iterator(char* value);

#endif
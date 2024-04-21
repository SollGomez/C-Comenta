#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "pthread.h"


extern int memoria_fd;
extern int cpu_fd;
extern int interfazIO_fd[4];
extern int kernel_fd;
extern t_log* info_logger;

int recibirConexion(char *);
void *recibirCPU(void);
void *recibirIO(int);
void *recibirKernel();
void cualInterfaz(int tipoInterfaz);

#endif
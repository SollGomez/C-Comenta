#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "pthread.h"


extern int memoria_fd;
extern int cpu_fd;
extern int filesystem_fd;
extern int kernel_fd;
extern t_log* info_logger;

int recibirConexion(char *);
void *recibirCPU(void);
void *recibirIO(void*);
void *recibirKernel();

#endif
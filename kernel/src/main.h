#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "comunicacion.h"
#include "pthread.h"
#include <commons/collections/queue.h>
#include <commons/config.h>
#include "init.h"

extern t_config* config;

void * conectarCPU(void *);
void *conectarCPUInterrupt(void *);
void * conectarMemoria(void *);
void *conectarFilesystem(void *);
t_config *crearConfig(char* configPath);

#endif /* MAIN_H_ */
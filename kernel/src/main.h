#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "comunicacion.h"
#include "pthread.h"
#include <commons/collections/queue.h>
#include <commons/config.h>
#include "init.h"
#include <utils/shared.h>
#include "planificadores.h"
#include "PCB.h"

extern t_config* config;
extern t_log* info_logger;

void * conectarCPU(void *);
void *conectarCPUInterrupt(void *);
void * conectarMemoria(void *);
void *escucharConexionesIO(void *);
t_config *crearConfig(char* configPath);

#endif /* MAIN_H_ */
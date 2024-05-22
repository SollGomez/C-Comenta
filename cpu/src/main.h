#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "comunicacion.h"
#include "cicloInstruccion.h"
#include "ejecucionInstrucciones.h"
#include "mmu.h"
#include <utils/utilidades.h>
#include "pthread.h"
#include <commons/collections/queue.h>
#include <commons/config.h>
#include <semaphore.h>

extern t_config* config;
extern t_log* info_logger;
extern t_log* error_logger;
extern sem_t bin_ciclo;

void *recibir();
void *conectarMemoria();
void *recibirInterrupt();
t_config *crearConfig(char* configPath);
void sigint_handler(int sig);

#endif /* MAIN_H_ */
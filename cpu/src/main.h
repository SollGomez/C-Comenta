#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "comunicacion.h"
#include "pthread.h"
#include <commons/collections/queue.h>
#include <commons/config.h>

extern t_config* config;
extern t_log* info_logger;
extern t_log* error_logger;

void *recibir();
void *conectarMemoria();
void *recibirInterrupt();
t_config *crearConfig(char* configPath);
void sigint_handler(int sig);

#endif /* MAIN_H_ */
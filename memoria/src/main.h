#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "comunicacion.h"
#include "pthread.h"
#include <commons/collections/queue.h>
#include<commons/config.h>

extern t_config* config;


void *recibir();
t_config *crearConfig(char* configPath);
void sigint_handler(int sig);

#endif /* MAIN_H_ */
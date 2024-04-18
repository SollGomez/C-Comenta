#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include <comunicacion.h>
#include "pthread.h"
#include <commons/collections/queue.h>
#include <commons/config.h>

extern t_config* config;

t_config *crearConfig(char* configPath);

#endif /* MAIN_H_ */
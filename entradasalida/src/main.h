#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include <comunicacion.h>
#include <stdbool.h>
#include <comunicacion.h>
#include <pthread.h>
#include <commons/collections/queue.h>
#include <commons/config.h>

extern t_config* config;

t_config *crearConfig(char* configPath);

#endif /* MAIN_H_ */
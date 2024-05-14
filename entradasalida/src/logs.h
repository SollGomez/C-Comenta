#ifndef LOGS_H_
#define LOGS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <string.h>
#include <pthread.h>
#include <utils/shared.h>
#include <semaphore.h>

extern pthread_mutex_t mutex_recvKernel;
extern pthread_mutex_t mutex_peticiones_pendientes;

extern t_list* lista_peticiones_pendientes;

extern sem_t sem_contador_peticiones;

typedef struct 
{
    char* TIPO_INTERFAZ;
    uint32_t TIEMPO_UNIDAD_TRABAJO;
    char* IP_KERNEL;
    char* PUERTO_KERNEL;
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PATH_BASE_DIALFS;
    uint32_t BLOCK_SIZE;
    uint32_t BLOCK_COUNT;
    uint32_t RETRASO_COMPACTACION;
    uint32_t TIPO_INTERFAZ_INT;

} t_config_entradaSalida;

int cargar_configuracion();
int init_loggers_config(char* path);

t_config_entradaSalida *cfg_entradaSalida_start();
void crearSemaforos();
void crearListas();
void logOperacion(uint32_t pid, char* operacionARealizar);


#endif /* LOGS_H_ */
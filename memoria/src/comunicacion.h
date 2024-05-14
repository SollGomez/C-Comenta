#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/utilidades.h>
#include "pthread.h"

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "main.h"
#include "esquema.h"
#include "memoriaInstrucciones.h"

extern t_config *config;

extern int memoria_fd;
extern int cpu_fd;
extern int interfazIO_fd[4];
extern int kernel_fd;
extern t_log* info_logger;
extern int RETARDO_RESPUESTA;
extern pthread_mutex_t mutexFS;

int recibirConexion(char *);
void *recibirCPU(void);
void *recibirIO(int);
void *recibirKernel();
void cualInterfaz(int tipoInterfaz);

void inicializarProceso(int);
void GuardarNombreArchiv(uint32_t pid, char* file_name);
void realizarPedidoLectura(int);
void realizarPedidoEscritura(int);

void finalizarProceso(int);
t_log* iniciar_logger(char*);
// void Paquete(int, t_log*);


// int conectarModulo(char *);
// void leer_consola(t_log*);
// void PaqueteHand(int, t_log*);
// void terminar_programa(int, t_log*);
// void iterator(char* value);
// void limpiarYEliminarListaAuxiliarPeroSinEliminarContenido(t_list*);
// bool enviarTablasPaginas(t_list*, int, t_log*,op_code_cliente);
// bool agregarTablasAPaquete(t_list*, t_paquete*);
// int finalizarProcesoConPid(uint32_t);
// void liberarModuloSwap(t_list*);
void realizarPedidoEscrituraInterfaz(int cliente_socket);
void realizarPedidoLecturaInterfaz(int cliente_socket);


#endif
#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include "pthread.h"
#include "main.h"

extern t_config* config;

extern int cpuDispatch_fd;
extern int cpuInterrupt_fd;
extern int memoria_fd;
extern int kernel_fd;
extern int interfazIO_fd[4];

int recibirConexion(char *puerto);
int conectarModuloCPU(char *);
int conectarModuloCPUInterrupt(char *);
int conectarModuloMemoria(char *);
void *recibirIO(int fdInterfaz);
void cualInterfaz(int tipoInterfaz);
void escucharCPU (void);

t_log* iniciar_logger(char*);
void leer_consola(t_log*);
void paquete(t_log*, char*);
void terminar_programa(int, t_log*);
void iterator(char* value);

PCB* obtenerPcbExec();
void actualizarPcbEjecucion(PCB* pcbRecibida);

void manejoDeRecursos(char* orden, char* recursoSolicitado);
void waitRecursoPcb(Recurso* recurso, PCB* unaPcb);
void signalRecursoPcb (Recurso* recurso, PCB* unaPcb);
void bloquearProcesoPorRecurso(Recurso* recurso);


#endif
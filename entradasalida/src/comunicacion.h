#ifndef UTILS_COMUNICACION_H_
#define UTILS_COMUNICACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/utilidades.h>
#include <pthread.h>
#include <manejarInterfaz.h>


typedef enum {
    EJECUTAR_IO_GEN_SLEEP,
    EJECUTAR_IO_STDOUT_WRITE,
    EJECUTAR_IO_STDIN_READ,
    EJECUTAR_IO_FS_CREATE,
    EJECUTAR_IO_FS_DELETE,
    EJECUTAR_IO_FS_READ,
    EJECUTAR_IO_FS_TRUNCATE,
    EJECUTAR_IO_FS_WRITE

    //Por ahora hasta ahí
}t_operacion_io;

typedef struct 
{
    t_operacion_io operacion;
    uint32_t unidadesDeTrabajo;

}t_peticion;





extern int contadorDispositivosIO;
extern int memoria_fd;
extern int kernel_fd;

int conectarKernel(char *modulo);
int conectarMemoria(char *modulo);
t_log* iniciar_logger(char*);
void paquete(int, t_log*);
void terminar_programa(int, t_log*);
void iterator(char* value);
void *recibirKernel();
void* solicitudIO_GEN_SLEEP (void* cliente_socket);
void agregarPeticionAPendientes(t_peticion* peticion_io_gen_sleep);
void iniciarAtencionPeticiones();
t_peticion* sacoPeticionDePendientes();
void atenderPeticiones();
void manejarPeticion(t_peticion* peticion);


#endif
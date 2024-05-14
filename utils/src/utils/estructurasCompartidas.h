#ifndef ESTRUCTURASCOMPARTIDAS_H_
#define ESTRUCTURASCOMPARTIDAS_H_

#include <stdbool.h>

#include "utilsClient.h"
#include "utilsServer.h"
#include <fcntl.h>

typedef struct {
	uint8_t idLength;
    char* id;
    uint8_t cantidadParametros;
    uint8_t param1Length;
    char* param1;
    uint8_t param2Length;
    char* param2;
    uint8_t param3Length;
    char* param3;
    uint8_t param4Length;
    char* param4;
    uint8_t param5Length;
    char* param5;
} Instruccion;

typedef struct {
    t_list* cola;
    char* nombreRecurso;
    int indiceSemaforo;
    int instanciasRecurso;
} Recurso;

typedef struct {
    char registro_AX[2];
    char registro_BX[2];
    char registro_CX[2];
    char registro_DX[2];
    char registro_EAX[5];
    char registro_EBX[5];
    char registro_ECX[5];
    char registro_EDX[5];
    char registro_SI[5];
    char registro_DI[5];
} RegistrosCPU;

typedef struct{
    uint32_t pid;
    t_list* paginas;
} TablaDePaginas;

typedef struct{
	uint32_t marco;
} Pagina;

typedef enum {
    NUEVO,
    LISTO,
    EJECUCION,
    BLOQUEADO,
    TERMINADO
} EstadoProceso;

typedef struct {
    char registro_AX[5];
    char registro_BX[5];
    char registro_CX[5];
    char registro_DX[5];
} RegistrosCPU;

typedef struct {
    uint32_t id;
    uint32_t prioridad;
    uint32_t program_counter;
    EstadoProceso estado;
    RegistrosCPU* registros;
    t_list* archivos_abiertos;
    t_list* listaInstrucciones;
    TablaDePaginas* tablaPaginas;
    t_list* recursosTomados;
    uint32_t tiempoEjecutando;
    uint32_t size;
    char* nombreRecurso;
} PCB;

typedef struct {
    char* nombreArchivo;
    pthread_mutex_t mutex;
    uint32_t modo;
    t_list* cola;
    t_list* colaWrite;
} t_archivo;

typedef struct {
    t_archivo* archivo;
    uint32_t ptro;
}t_archivoLocal;

typedef struct {
    t_archivo* archivo;
    PCB* pcb;
} t_archivoPeticion;

typedef struct {
    uint32_t nombreArchivoLength;
    char* nombreArchivo;
    uint32_t nuevoTamanio;
}t_archivoTruncate;

typedef struct {
    uint32_t nombreArchivoLength;
    char* nombreArchivo;
    uint32_t posPuntero;
    uint32_t direcFisica;
    uint32_t cantidadBytes;
    uint32_t pid;
}t_archivoRW;

typedef struct{
    uint32_t tamanio;
    void* datos;
} t_datos;

#endif
#ifndef ESTRUCTURASCOMPARTIDAS_H_
#define ESTRUCTURASCOMPARTIDAS_H_

#include <stdbool.h>

#include "utilsClient.h"
#include "utilsServer.h"
#include <fcntl.h>

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

typedef struct {
    uint32_t id;
    uint32_t program_counter;
    uint32_t quantum;
    RegistrosCPU* registros;
    t_list* listaInstrucciones;
    TablaDePaginas* tablaPaginas;
    t_list* recursosTomados;
    uint32_t tiempoEjecutando;
    uint32_t size;
    char* nombreRecurso;
} PCB;


#endif
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
} Instruccion;

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

typedef struct{
    uint32_t tamanio;
    void* datos;
} t_datos;

typedef struct{

	uint32_t pc;//Program Counter por el que es apuntado
	char* operador;//Token del operador
	char* operando1;//Valor del operando 1
	char* operando2;//Valor del operando 2

}InstruccionPseudo;//Estructura de una instrucción

#endif
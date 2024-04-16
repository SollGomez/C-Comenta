#ifndef ESQUEMA_H_
#define ESQUEMA_H_
#include "memoriaInstrucciones.h"
#include "main.h"

typedef struct {
    TablaDePaginas* procesos;
    size_t numProcesos;
} TablaGeneral;

typedef struct{
	int tamanioMemoria;
	void *espacioDisponible;
	int memoriaIniciada;
}espacioContiguoMemoria;

uint32_t buscarMarcoLibre(); //Devuelve el primer marco libre que encuentra
void marcarMarcoOcupado(int); //Marca como ocupado el marco que le pasemos
void marcarMarcoLibre(int); //Marca como libre el marco que le pasemos
void crearTablaPaginasProceso(uint32_t, uint32_t); //Crea la tabla de paginas de un proceso y la agrega a la TablaGeneral
void finalizacionDeProceso(uint32_t); //Marca como libres los marcos del proceso
TablaDePaginas* obtenerTablaPorPID(uint32_t); //Devuelve la tabla cuyo PID sea el pedido
void liberarTablaDePaginas(uint32_t); //Elimina la tabla del proceso de la TablaGeneral
uint32_t obtenerMarcoDePagina(uint32_t, uint32_t); //Devuelve el numero de marco de la pagina solicitada

extern espacioContiguoMemoria espacioUsuario;
extern TablaDePaginas tablaDePaginas;
extern int tam_pagina;
extern bool flagComunicacion;

#endif /* ESQUEMA_H_ */
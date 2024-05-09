#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include "estructurasCompartidas.h"
#include "utilsServer.h"

extern int idProcesoGlobal;

//REVISAR TODAS

t_list* recibirListaUint32_t(int);
void simularRetardoSinMensaje(int);
void enviarValor_uint32(uint32_t, int, op_code_cliente, t_log *);
void agregar_instruccion_a_paquete(t_paquete *paquete, Instruccion* instruccion); //VER porque esta fea... es nueva
bool agregarIntsYDatosAPaquete(t_list* listaInts, t_datos* datos, t_paquete* paquete);
bool enviarListaIntsYDatos(t_list* listaInts,t_datos* datos, int socket_cliente, t_log* logger, op_code_cliente codigo);
t_list* recibirListaIntsYDatos(int socket_cliente,t_datos* datos);
void enviarOrden(op_code_cliente orden, int socket, t_log *logger);
char* recibirEnteroEnteroChar(int socket_cliente, uint32_t* entero1, uint32_t* entero2);
uint32_t recibirValor_uint32(int socket);

#endif
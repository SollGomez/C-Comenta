#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include "shared.h"

Instruccion* recibirInstruccion(int conexion);
bool agregarUint32_tsAPaquete(t_list*, t_paquete*);
bool enviarListaUint32_t(t_list*, int, t_log*, op_code_cliente);
t_list* recibirListaUint32_t(int socket_cliente);
void enviarValor_uint32(uint32_t valor, int socket, op_code_cliente orden, t_log *logger);
uint32_t recibirValor_uint32(int socket);
void* recibir_stream(int* size, uint32_t cliente_socket);
bool enviarEnteroYString(uint32_t entero,char* string, int socket_cliente, t_log* logger, op_code_cliente codigo);
bool agregarEnteroYStringAPaquete(uint32_t entero, char* string, t_paquete* paquete);
char* recibirEnteroYString(int socket_cliente,uint32_t* entero);
#endif
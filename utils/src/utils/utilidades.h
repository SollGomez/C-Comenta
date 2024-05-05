#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include "shared.h"

Instruccion* recibirInstruccion(int conexion);
void liberarInstruccion(Instruccion * instruccion);
bool agregarUint32_tsAPaquete(t_list*, t_paquete*);
bool enviarListaUint32_t(t_list*, int, t_log*, op_code_cliente);

void agregar_registros_a_paquete(t_paquete* paquete, RegistrosCPU* registro);
void agregar_ContextoEjecucion_a_paquete(t_paquete *paquete, PCB* pcb);
PCB* recibir_contextoEjecucion(int conexion);
void liberarPcbCpu(PCB* pcb);
void enviar_uint32_y_uint32_y_char(char* path, uint32_t valor1, uint32_t valor2, int socket, op_code_cliente orden, t_log *logger);
void recibirOrden(int socket);
void enviarOrden(op_code_cliente orden, int socket, t_log *logger);
void enviarValor_uint32(uint32_t valor, int socket, op_code_cliente orden, t_log *logger);
void* recibir_stream(int* size, uint32_t cliente_socket);
PCB* recibir_contextoEjecucion_y_char(int conexion);

#endif
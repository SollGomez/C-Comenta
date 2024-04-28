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

#endif
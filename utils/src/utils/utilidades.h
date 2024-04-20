#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include "shared.h"

Instruccion* recibirInstruccion(int conexion);
bool agregarUint32_tsAPaquete(t_list*, t_paquete*);
bool enviarListaUint32_t(t_list*, int, t_log*, op_code_cliente);

#endif
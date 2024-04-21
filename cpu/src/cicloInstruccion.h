#ifndef CICLOINSTRUCCION_H_
#define CICLOINSTRUCCION_H_

#include "main.h"

extern bool cicloInstrucciones;
extern int interrupciones;

extern char registroCPU_AX[1];
extern char registroCPU_BX[1];
extern char registroCPU_CX[1];
extern char registroCPU_DX[1];
extern char registroCPU_EAX[4];
extern char registroCPU_EBX[4];
extern char registroCPU_ECX[4];
extern char registroCPU_EDX[4];
extern char registroCPU_SI[4];
extern char registroCPU_DI[4];

void ciclo_de_instruccion();
void fetch();
void decode();
void execute();
void checkInsterrupt();

void copiar_registroPCB_a_los_registrosCPU (RegistrosCPU* registro);
void copiar_registrosCPU_a_los_registroPCB(RegistrosCPU* registro);

#endif
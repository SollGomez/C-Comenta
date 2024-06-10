#ifndef EJECUCIONINSTRUCCIONES_H_
#define EJECUCIONINSTRUCCIONES_H_

#include "main.h"

void SET(char*, char*);
void SUM(char*, char*);
void SUB(char*, char*);
void JNZ(char*, char*);
void resize(char*);
void COPY_STRING(char*);

void ejecutar_WAIT(char* );
void ejecutar_SIGNAL(char*);

void ejecutar_MOV_IN(char*, int);
void ejecutar_MOV_OUT(int, char*);

void ioGenSleep(char* interfaz, char* unidadesDeTrabajo);
void ioStdinRead(char* interfaz, char* registroDireccion, char* registroTamanio);
void ioStdOutWrite(char* interfaz, char* registroDireccion, char* registroTamanio);
void ioFsCreate(char* interfaz, char* nombreArchivo);
void ioFsDelete(char* interfaz, char* nombreArchivo);
void ioFsTruncate(char* interfaz, char* nombreArchivo, char* registroTamanio);
void ioFsWrite(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo);
void ioFsRead(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo);

void ejecutar_EXIT();

void cambiar_valor_del_registroCPU(char* registro, char* valor);
int obtener_valor_registroCPU(char* registro);

#endif
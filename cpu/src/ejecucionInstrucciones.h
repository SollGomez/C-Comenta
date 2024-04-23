#ifndef EJECUCIONINSTRUCCIONES_H_
#define EJECUCIONINSTRUCCIONES_H_

#include "main.h"

void SET(char*, char*);
void SUM(char*, char*);
void SUB(char*, char*);
void JNZ(char*, char*);
void RESIZE(char*);
void COPY_STRING(char*);

void ejecutar_WAIT(char* );
void ejecutar_SIGNAL(char*);

void ejecutar_MOV_IN(char*, int);
void ejecutar_MOV_OUT(int, char*);

void IO_GEN_SLEEP(char* interfaz, char* unidadesDeTrabajo);
void IO_STDIN_READ(char* interfaz, char* registroDireccion, char* registroTamanio);
void IO_STDOUT_WRITE(char* interfaz, char* registroDireccion, char* registroTamanio);
void IO_FS_CREATE(char* interfaz, char* nombreArchivo);
void IO_FS_DELETE(char* interfaz, char* nombreArchivo);
void IO_FS_TRUNCATE(char* interfaz, char* nombreArchivo, char* registroTamanio);
void IO_FS_WRITE(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo);
void IO_FS_READ(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo);

void ejecutar_EXIT();

#endif
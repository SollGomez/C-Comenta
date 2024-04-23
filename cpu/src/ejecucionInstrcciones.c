#include "ejecucionInstrucciones.h"

void SET(char* registro, char* valor){
	
}


void SUM(char* value1, char* value2) {

}

void SUB(char* value1, char* value2) {

}

void JNZ(char* registro, char* numeroInstruccion){
	
}

void RESIZE(char* tamanio){

}

void COPY_STRING(char* tamanio){

}

void ejecutar_WAIT(char* nombre_recurso) {
	
}

void ejecutar_SIGNAL(char* nombre_recurso) {
	
}

void ejecutar_MOV_IN(char* registro, int direccion_logica) {
	
}


void ejecutar_MOV_OUT(int direccion_logica, char* registro) {
	
}

void IO_GEN_SLEEP(char* interfaz, char* unidadesDeTrabajo){

}

void IO_STDIN_READ(char* interfaz, char* registroDireccion, char* registroTamanio){

}

void IO_STDOUT_WRITE(char* interfaz, char* registroDireccion, char* registroTamanio){

}

void IO_FS_CREATE(char* interfaz, char* nombreArchivo){

}

void IO_FS_DELETE(char* interfaz, char* nombreArchivo){

}

void IO_FS_TRUNCATE(char* interfaz, char* nombreArchivo, char* registroTamanio){

}

void IO_FS_WRITE(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo){

}

void IO_FS_READ(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo){

}

void ejecutar_EXIT(){
	
}
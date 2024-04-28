#include "ejecucionInstrucciones.h"

void SET(char* registro, char* valor){
	cambiar_valor_del_registroCPU(registro,valor);
    PCB_Actual->program_counter++;
}


void SUM(char* value1, char* value2) {
    int suma=0;
	suma+=atoi(obtener_valor_registroCPU(value1));
	suma+=atoi(obtener_valor_registroCPU(value2));

	char buffer[20];
	sprintf(buffer, "%d", suma);

    SET(value1, buffer);
}

void SUB(char* value1, char* value2) {
	int resta=0;
	resta+=atoi(obtener_valor_registroCPU(value1));
	resta-=atoi(obtener_valor_registroCPU(value2));

	char buffer[20];
	sprintf(buffer, "%d", resta);

	SET(value1, buffer);
}

void JNZ(char* registro, char* numeroInstruccion){
	if(atoi(obtener_valor_registroCPU(registro)))
		PCB_Actual->program_counter=atoi(numeroInstruccion);
	else
		PCB_Actual->program_counter++;
}

void RESIZE(char* tamanio){

}

void COPY_STRING(char* tamanio){
    memcpy(registroCPU_DI, registroCPU_SI, atoi(tamanio));
    PCB_Actual->program_counter++;
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

void cambiar_valor_del_registroCPU(char* registro, char* valor) {
    if (strcmp(registro, "AX") == 0)
        memcpy(registroCPU_AX, valor, 1);

    if (strcmp(registro, "BX") == 0)
        memcpy(registroCPU_BX, valor, 1);

    if (strcmp(registro, "CX") == 0)
        memcpy(registroCPU_CX, valor,1);

    if (strcmp(registro, "DX") == 0)
        memcpy(registroCPU_DX, valor, 1);

    if (strcmp(registro, "EAX") == 0)
        memcpy(registroCPU_AX, valor, 4);

    if (strcmp(registro, "EBX") == 0)
        memcpy(registroCPU_BX, valor, 4);

    if (strcmp(registro, "ECX") == 0)
        memcpy(registroCPU_CX, valor,4);

    if (strcmp(registro, "EDX") == 0)
        memcpy(registroCPU_DX, valor, 4);

    if (strcmp(registro, "SI") == 0)
        memcpy(registroCPU_DX, valor, 4);
    
    if (strcmp(registro, "DI") == 0)
        memcpy(registroCPU_DX, valor, 4);
}

char* obtener_valor_registroCPU(char* registro) {


    if (strcmp(registro, "AX") == 0){

         char* valor = (char *) malloc (1 + 1);
         strcpy (valor,"p");
         memcpy(valor,registroCPU_AX,1);

        return valor;
    }
    if (strcmp(registro, "BX") == 0) {

         char* valor = (char *) malloc (1 + 1);
         strcpy (valor,"p");
        memcpy(valor,registroCPU_BX, 1);

        return valor;
     }
    if (strcmp(registro, "CX") == 0) {

        char* valor = (char *) malloc (1 + 1);
        strcpy (valor,"p");
        memcpy(valor, registroCPU_CX, 1);

        return valor;
     }
    if (strcmp(registro, "DX") == 0) {
        char* valor = (char *) malloc (1 + 1);
        strcpy (valor,"p");
        memcpy(valor, registroCPU_DX, 1);

        return valor;
     }
    if (strcmp(registro, "EAX") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_EAX, 4);

        return valor;
     }
    if (strcmp(registro, "EBX") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_EBX, 4);

        return valor;
     }
    if (strcmp(registro, "ECX") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_ECX, 4);

        return valor;
     }
    if (strcmp(registro, "EDX") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_EDX, 4);

        return valor;
     }
    if (strcmp(registro, "SI") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_SI, 4);

        return valor;
     }
    if (strcmp(registro, "DI") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_DI, 4);

        return valor;
     }

}
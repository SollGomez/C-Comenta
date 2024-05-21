#include "ejecucionInstrucciones.h"

void SET(char* registro, char* valor){
	cambiar_valor_del_registroCPU(registro,valor);
    PCB_Actual->program_counter++;
}


void SUM(char* value1, char* value2) {
    int suma=0;
    log_info(info_logger, "llega");
	suma+=atoi(obtener_valor_registroCPU(value1));
    log_info(info_logger, "llega1 %d %s", suma, obtener_valor_registroCPU(value2));
	suma+=atoi(obtener_valor_registroCPU(value2));
    log_info(info_logger, "llega2 %d", suma);

	char buffer[20];
	sprintf(buffer, "%d", suma);

    SET(value1, buffer);
}

void SUB(char* value1, char* value2) {
	int resta=0;
	resta+=atoi(obtener_valor_registroCPU(value1));
	resta-=atoi(obtener_valor_registroCPU(value2));
    log_info(info_logger, "llega2 %d", resta);

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

void resize(char* tamanio){

}

void COPY_STRING(char* tamanio){
    memcpy(registroCPU_DI, registroCPU_SI, atoi(tamanio));
    PCB_Actual->program_counter++;
}

void ejecutar_WAIT(char* nombre_recurso) {
	copiar_registrosCPU_a_los_registroPCB(PCB_Actual->registros);
    PCB_Actual->program_counter++;
    t_paquete* paquete = crear_paquete(WAIT, info_logger);
    agregar_ContextoEjecucion_a_paquete(paquete, PCB_Actual);
    uint32_t largo_nombre = strlen(nombre_recurso) + 1;
    agregar_a_paquete2(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete2(paquete, nombre_recurso, largo_nombre);
    enviar_paquete(paquete, kernel_fd);
    eliminar_paquete(paquete);
    cicloInstrucciones = false;
}

void ejecutar_SIGNAL(char* nombre_recurso) {
	copiar_registrosCPU_a_los_registroPCB(PCB_Actual->registros);
    PCB_Actual->program_counter++;
    t_paquete* paquete = crear_paquete(SIGNAL, info_logger);
    agregar_ContextoEjecucion_a_paquete(paquete, PCB_Actual);
    uint32_t largo_nombre = strlen(nombre_recurso) + 1;
    agregar_a_paquete2(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete2(paquete, nombre_recurso, largo_nombre);
    enviar_paquete(paquete, kernel_fd);
    eliminar_paquete(paquete);
    cicloInstrucciones = false;
}

void ejecutar_MOV_IN(char* registro, int direccion_logica) {
	 int direccion_fisica = traducir_direccion_logica(direccion_logica);

    if (!(direccion_fisica < 0)) {
    	   char* valor;
           //valor = leer_valor_de_memoria(direccion_fisica);
           cambiar_valor_del_registroCPU(registro,valor);
           free(valor);
           PCB_Actual->program_counter++;
    }
}


void ejecutar_MOV_OUT(int direccion_logica, char* registro) {
	char* valorDelRegistro = obtener_valor_registroCPU(registro);
    int direccion_fisica = traducir_direccion_logica(direccion_logica);

    if (!(direccion_fisica < 0)) {
        //escribir_valor_en_memoria(direccion_fisica, valorDelRegistro);
        PCB_Actual->program_counter++;
    }
    free(valorDelRegistro);
}

void ioGenSleep(char* interfaz, char* unidadesDeTrabajo){
    uint32_t numeroInterfaz = 0;
    uint32_t tiempoEspera = atoi(unidadesDeTrabajo);

    if(!strcmp(interfaz,"STDOUT"))
        numeroInterfaz=0;
    if(!strcmp(interfaz,"STDIN"))
        numeroInterfaz=1;
    if(!strcmp(interfaz,"DIAL_FS"))
        numeroInterfaz=2;
    if(!strcmp(interfaz,"GENERICA"))
        numeroInterfaz=3;

    copiar_registrosCPU_a_los_registroPCB(PCB_Actual->registros);
    PCB_Actual->program_counter++;
    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP_OPC, info_logger);
    agregar_ContextoEjecucion_a_paquete(paquete, PCB_Actual);
    agregar_a_paquete2(paquete, &numeroInterfaz, sizeof(uint32_t)); //Diseñar para Kernel recibir contexto ejecución y uint y uint
    agregar_a_paquete2(paquete, &tiempoEspera, sizeof(uint32_t));
    enviar_paquete(paquete, kernel_fd);
    eliminar_paquete(paquete);
    //cicloInstrucciones = false;
}

void ioStdinRead(char* interfaz, char* registroDireccion, char* registroTamanio){

}

void ioStdOutWrite(char* interfaz, char* registroDireccion, char* registroTamanio){

}

void ioFsCreate(char* interfaz, char* nombreArchivo){

}

void ioFsDelete(char* interfaz, char* nombreArchivo){

}

void ioFsTruncate(char* interfaz, char* nombreArchivo, char* registroTamanio){

}

void ioFsWrite(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo){

}

void ioFsRead(char* interfaz, char* nombreArchivo, char* registroDireccion, char* registroTamanio, char* registroPunteroArchivo){

}

void ejecutar_EXIT(){
	copiar_registrosCPU_a_los_registroPCB(PCB_Actual->registros);
    t_paquete* paquete = crear_paquete(EXIT, info_logger);
    agregar_ContextoEjecucion_a_paquete(paquete, PCB_Actual);
    enviar_paquete(paquete, kernel_fd);
    eliminar_paquete(paquete);
    cicloInstrucciones = false;
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
		// if (!strncmp(instruccion,"EJECUTAR_SCRIPT", strlen("EJECUTAR_SCRIPT")))
	    // 	ejecutar_script(instruccion);

    if (!strncmp(registro, "AX", strlen("AX"))){

        char* valor = (char *) malloc (1 + 1);
        strcpy (valor,"p");
        memcpy(valor,registroCPU_AX,1);

        return valor;
    }
    if (!strncmp(registro, "BX", strlen("BX"))) {

        char* valor = (char *) malloc (1 + 1);
        strcpy (valor,"p");
        memcpy(valor,registroCPU_BX, 1);

        return valor;
     }
    if (!strncmp(registro, "CX", strlen("CX"))) {

        char* valor = (char *) malloc (1 + 1);
        strcpy (valor,"p");
        memcpy(valor, registroCPU_CX, 1);

        return valor;
     }
    if (!strncmp(registro, "DX", strlen("DX"))) {
        char* valor = (char *) malloc (1 + 1);
        strcpy (valor,"p");
        memcpy(valor, registroCPU_DX, 1);

        return valor;
     }
    if (!strncmp(registro, "EAX", strlen("EAX"))) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_EAX, 4);

        return valor;
     }
    if (!strncmp(registro, "EBX", strlen("EBX"))) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_EBX, 4);

        return valor;
     }
    if (!strncmp(registro, "ECX", strlen("ECX"))) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_ECX, 4);

        return valor;
     }
    if (!strncmp(registro, "EDX", strlen("EDX"))) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_EDX, 4);

        return valor;
     }
    if (!strncmp(registro, "SI", strlen("SI"))) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_SI, 4);

        return valor;
     }
    if (!strncmp(registro, "DI", strlen("DI"))) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_DI, 4);

        return valor;
     }

}
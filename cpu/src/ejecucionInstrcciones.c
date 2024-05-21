#include "ejecucionInstrucciones.h"

void SET(char* registro, char* valor){
	cambiar_valor_del_registroCPU(registro,valor);
    PCB_Actual->program_counter++;
}


void SUM(char* value1, char* value2) {
    int suma=0;
    log_info(info_logger, "llega");
	suma+=obtener_valor_registroCPU(value1);
    log_info(info_logger, "llega1 %d %d", suma, obtener_valor_registroCPU(value2));
	suma+=obtener_valor_registroCPU(value2);
    log_info(info_logger, "llega2 %d", suma);

	char buffer[20];
	sprintf(buffer, "%d", suma);

    SET(value1, buffer);
}

void SUB(char* value1, char* value2) {
	int resta=0;
	resta+=obtener_valor_registroCPU(value1);
	resta-=obtener_valor_registroCPU(value2);
    log_info(info_logger, "llega2 %d", resta);

	char buffer[20];
	sprintf(buffer, "%d", resta);

	SET(value1, buffer);
}

void JNZ(char* registro, char* numeroInstruccion){
	if(obtener_valor_registroCPU(registro))
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
    
    
    t_list* listaInts = list_create();

	list_add(listaInts, &numeroInterfaz);
	list_add(listaInts, &tiempoEspera);
    list_add(listaInts, &PCB_Actual->id);

	enviarListaUint32_t(listaInts,kernel_fd, info_logger, IO_GEN_SLEEP_OPC);

    list_clean(listaInts);
	list_destroy(listaInts);
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
    if (strcmp(registro, "PC") == 0)
        PCB_Actual->program_counter = atoi(valor);

    if (strcmp(registro, "AX") == 0)
        registroCPU_AX = atoi(valor);

    if (strcmp(registro, "BX") == 0)
        registroCPU_BX = atoi(valor);

    if (strcmp(registro, "CX") == 0)
        registroCPU_CX = atoi(valor);

    if (strcmp(registro, "DX") == 0)
        registroCPU_DX = atoi(valor);

    if (strcmp(registro, "EAX") == 0)
        registroCPU_EAX = atoi(valor);

    if (strcmp(registro, "EBX") == 0)
        registroCPU_EBX = atoi(valor);

    if (strcmp(registro, "ECX") == 0)
        registroCPU_ECX = atoi(valor);

    if (strcmp(registro, "EDX") == 0)
        registroCPU_EDX = atoi(valor);

    if (strcmp(registro, "SI") == 0)
        registroCPU_SI = atoi(valor);
    
    if (strcmp(registro, "DI") == 0)
        registroCPU_DI = atoi(valor);
}

int obtener_valor_registroCPU(char* registro) {

    if (!strncmp(registro, "AX", strlen("AX"))){
        return registroCPU_AX;
    }
    if (!strncmp(registro, "BX", strlen("BX"))) {
        return registroCPU_BX;
     }
    if (!strncmp(registro, "CX", strlen("CX"))) {
        return registroCPU_CX;
     }
    if (!strncmp(registro, "DX", strlen("DX"))) {
        return registroCPU_DX;
     }
    if (!strncmp(registro, "EAX", strlen("EAX"))) {
        return registroCPU_EAX;
     }
    if (!strncmp(registro, "EBX", strlen("EBX"))) {
        return registroCPU_EBX;
     }
    if (!strncmp(registro, "ECX", strlen("ECX"))) {
        return registroCPU_ECX;
     }
    if (!strncmp(registro, "EDX", strlen("EDX"))) {
        return registroCPU_EDX;
     }
    if (!strncmp(registro, "SI", strlen("SI"))) {
        return registroCPU_SI;
     }
    if (!strncmp(registro, "DI", strlen("DI"))) {
        return registroCPU_DI;
     }

}
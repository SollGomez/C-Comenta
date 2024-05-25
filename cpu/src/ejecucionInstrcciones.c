#include "ejecucionInstrucciones.h"

void SET(char* registro, char* valor){
	cambiar_valor_del_registroCPU(registro,valor);
    PCB_Actual->program_counter++;
}


void SUM(char* value1, char* value2) {
    int suma=0;
	suma+=obtener_valor_registroCPU(value1);
	suma+=obtener_valor_registroCPU(value2);

	char buffer[20];
	sprintf(buffer, "%d", suma);

    SET(value1, buffer);
}

void SUB(char* value1, char* value2) {
	int resta=0;
	resta+=obtener_valor_registroCPU(value1);
	resta-=obtener_valor_registroCPU(value2);

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
    int cantidad_bytes = calcular_bytes_segun_registro(registro);
	int direccion_fisica = traducir_direccion_logica(direccion_logica);

    if (!(direccion_fisica < 0)) {
    	   char* valor;
           valor = leer_valor_de_memoria(direccion_fisica, cantidad_bytes);
           cambiar_valor_del_registroCPU(registro,valor);
           free(valor);
           PCB_Actual->program_counter++;
    }
}


void ejecutar_MOV_OUT(int direccion_logica, char* registro) {
    int cantidad_bytes = calcular_bytes_segun_registro(registro);
	int valorDelRegistro = obtener_valor_registroCPU(registro);
    int direccion_fisica = traducir_direccion_logica(direccion_logica);

    char buffer[20];
	sprintf(buffer, "%d", valorDelRegistro);

    if (!(direccion_fisica < 0)) {
        escribir_valor_en_memoria(direccion_fisica,cantidad_bytes, buffer);
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
    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP, info_logger);
    agregar_ContextoEjecucion_a_paquete(paquete, PCB_Actual);
    agregar_a_paquete2(paquete, &numeroInterfaz, sizeof(uint32_t)); //Diseñar para Kernel recibir contexto ejecución y uint y uint
    agregar_a_paquete2(paquete, &tiempoEspera, sizeof(uint32_t));
    enviar_paquete(paquete, kernel_fd);
    eliminar_paquete(paquete);
    cicloInstrucciones = false;
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

char* leer_valor_de_memoria(int direccion_fisica, int bytesRegistro) {

    t_list* listaInts = list_create();
    uint32_t uint32t_dir_fis = direccion_fisica;
    uint32_t uint32t_tamanio = bytesRegistro; 

    list_add(listaInts, &uint32t_dir_fis);
    list_add(listaInts, &uint32t_tamanio);
    list_add(listaInts, &PCB_Actual->id);

    enviarListaUint32_t(listaInts,memoria_fd, info_logger, ACCESO_PEDIDO_LECTURA);
    char* valor = recibir_valor_de_memoria();
    list_clean(listaInts);
    list_destroy(listaInts);
    log_info(info_logger, "PID: <%d> - Acción: <LEER> - Dirección Fisica: <%d> - Valor: <%s>", PCB_Actual->id, direccion_fisica, valor);

    return valor;
 }

char* recibir_valor_de_memoria(){

        char* valor;
        int cod_op = recibir_operacion(memoria_fd);

		switch (cod_op) {
		case LECTURA_REALIZADA:{
            t_datos* unosDatos = malloc(sizeof(t_datos));
            t_list* listaInts = recibirListaIntsYDatos(memoria_fd,unosDatos);
            uint32_t tamanio = *(uint32_t*)list_get(listaInts,1);
            valor = malloc(unosDatos->tamanio+1);
            memcpy(valor,unosDatos->datos,tamanio);
            valor[tamanio] = '\0';
            free(unosDatos->datos);
            free(unosDatos);
            list_clean_and_destroy_elements(listaInts,free);
            list_destroy(listaInts);

            break;
        }
        default:
                log_error(error_logger, "No se recibio el valor correctamente, cerrando el programa");
                exit(1);  //TODO: Hay que cerrar como se debe
        }
    return valor;
}

void escribir_valor_en_memoria(int direccion_fisica, int cantidad_bytes, char* valor){


    t_list* listaInts = list_create();
    t_datos* unosDatos = malloc(sizeof(t_datos));
    unosDatos->tamanio= cantidad_bytes;
    unosDatos->datos = (void*) valor;
    list_add(listaInts, &direccion_fisica);
    list_add(listaInts, &PCB_Actual->id);

    enviarListaIntsYDatos(listaInts, unosDatos, memoria_fd, info_logger, ACCESO_PEDIDO_ESCRITURA);
    list_clean(listaInts);
    list_destroy(listaInts);

    char* valor2 = recibir_confirmacion_de_escritura() ;
    if (strcmp(valor2, "OK") == 0) {
        log_info(info_logger, "PID: <%d> - Accion: <ESCRIBIR> - Dirección Fisica: <%d> - Valor: <%s>", PCB_Actual->id, direccion_fisica, valor);
    }
    free(unosDatos);

 }

 char*  recibir_confirmacion_de_escritura(){

        char* valor; 
        int cod_op = recibir_operacion(memoria_fd);

		switch (cod_op) {
		case ESCRITURA_REALIZADA:
             recibirOrden(memoria_fd);
             valor= "OK";
			 break;
        }
        return valor;
}

int calcular_bytes_segun_registro(char* registro){
    int bytes;

    if ((strcmp(registro, "AX") == 0)||(strcmp(registro, "BX") == 0)||(strcmp(registro, "CX") == 0)||(strcmp(registro, "DX") == 0))
        bytes = 1;
    if ((strcmp(registro, "EAX") == 0)||(strcmp(registro, "EBX") == 0)||(strcmp(registro, "ECX") == 0)||(strcmp(registro, "EDX") == 0)||(strcmp(registro, "SI") == 0)||(strcmp(registro, "DI") == 0))
        bytes = 4;

    return bytes;
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
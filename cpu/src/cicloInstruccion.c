#include "cicloInstruccion.h"

bool cicloInstrucciones=true;
char* nombre_instruccion_actual;
Instruccion* instruccion;
int interrupciones;


char registroCPU_AX[1];
char registroCPU_BX[1];
char registroCPU_CX[1];
char registroCPU_DX[1];
char registroCPU_EAX[4];
char registroCPU_EBX[4];
char registroCPU_ECX[4];
char registroCPU_EDX[4];
char registroCPU_SI[4];
char registroCPU_DI[4];

void ciclo_de_instruccion(){
    
    cicloInstrucciones=true;
    interrupciones=0;

    while(cicloInstrucciones){

        fetch();

        decode();

        execute();

        checkInsterrupt();

    }
}

void fetch(){
    t_list* listaInts = list_create();

	log_info(info_logger, "PID: <%d> - FETCH - Program Counter: <%d>", PCB_Actual->id, PCB_Actual->program_counter);
	
	list_add(listaInts, &PCB_Actual->id);
	list_add(listaInts, &PCB_Actual->program_counter);


	enviarListaUint32_t(listaInts,memoria_fd, info_logger, SOLICITUDINSTRUCCION);

	list_clean(listaInts);
	list_destroy(listaInts);
}

void decode(){
    t_list* lista;
	op_code_cliente cod = recibir_operacion(memoria_fd);

	if(cod == SOLICITUDINSTRUCCION){        //en memoria mandar enviar instruccion
		instruccion = recibirInstruccion(memoria_fd);

		nombre_instruccion_actual=instruccion->id;
		log_info(info_logger,"instruccion recibida: %s", nombre_instruccion_actual);

	}
	else{
		log_error(error_logger,"FALLO en el recibo de la instruccion");
		cicloInstrucciones=0; //si hay pageFault tmbn tiene que devolver -1
	}
}

void execute(){
	if (strcmp(nombre_instruccion_actual , "SET") == 0) {
        char* registro = instruccion->param1;
        char* valor = instruccion->param2;

        log_info(info_logger, "PID: <%d> - Ejecutando: <SET> - <%s> - <%s>", PCB_Actual->id, registro, valor);

        SET(registro, valor);

    }
    else if (strcmp(nombre_instruccion_actual , "SUM") == 0) {
		char* registro = instruccion->param1;
		char* valor = instruccion->param2;

		log_info(info_logger, "PID: <%d> - Ejecutando: <SUM> - <%s> - <%s>", PCB_Actual->id, registro, valor);

		SUM(registro, valor);

	}
    else if (strcmp(nombre_instruccion_actual , "SUB") == 0) {
		char* registro = instruccion->param1;
		char* valor = instruccion->param2;

		log_info(info_logger, "PID: <%d> - Ejecutando: <SUB> - <%s> - <%s>", PCB_Actual->id, registro, valor);

		SUB(registro, valor);

	}
    else if (strcmp(nombre_instruccion_actual , "JNZ") == 0) {
    		char* registro = instruccion->param1;
    		char* valor = instruccion->param2;

    		log_info(info_logger, "PID: <%d> - Ejecutando: <JNZ> - <%s> - <%s>", PCB_Actual->id, registro, valor);

    		JNZ(registro, valor);

    	}
    else if (strcmp(nombre_instruccion_actual , "RESIZE") == 0) {
		char* tamanio = instruccion->param1;

		log_info(info_logger, "PID: <%d> - Ejecutando: <RESIZE> - <%s>", PCB_Actual->id, tamanio);

		RESIZE(tamanio);

	}
	else if (strcmp(nombre_instruccion_actual , "COPY_STRING") == 0) {
		char* tamanio = instruccion->param1;

		log_info(info_logger, "PID: <%d> - Ejecutando: <COPY_STRING> - <%s>", PCB_Actual->id, tamanio);

		COPY_STRING(tamanio);

	}
    else if (strcmp(nombre_instruccion_actual, "WAIT") == 0) {
		char* recurso = instruccion->param1;
		log_info(info_logger, "PID: <%d> - Ejecutando <WAIT> - <%s>", PCB_Actual->id, recurso);

		ejecutar_WAIT(recurso);
	}
	else if (strcmp(nombre_instruccion_actual, "SIGNAL") == 0) {
		char* recurso = instruccion->param1;
		log_info(info_logger, "PID: <%d> - Ejecutando <SIGNAL> - <%s>", PCB_Actual->id, recurso);

		ejecutar_SIGNAL(recurso);
	}
    else if (strcmp(nombre_instruccion_actual, "MOV_IN") == 0) {
        char* registro = instruccion->param1;
        int direccion_logica = atoi(instruccion->param2);
        log_info(info_logger, "PID: <%d> - Ejecutando: <MOV_IN> - <%s> - <%d>", PCB_Actual->id, registro, direccion_logica);

        ejecutar_MOV_IN(registro, direccion_logica);
    }

    else if (strcmp(nombre_instruccion_actual, "MOV_OUT") == 0) {

        int direccion_logica = atoi(instruccion->param1);
        char* registro = instruccion->param2;

        log_info(info_logger, "PID: <%d> - Ejecutando: <MOV_OUT> - <%d> - <%s>", PCB_Actual->id, direccion_logica, registro);

        ejecutar_MOV_OUT(direccion_logica, registro);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_GEN_SLEEP") == 0) {

        char* interfaz = instruccion->param1;
        char* unidadesDeTrabajo = instruccion->param2;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_GEN_SLEEP> - <%d> - <%s>", PCB_Actual->id, interfaz, unidadesDeTrabajo);

		//Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
        IO_GEN_SLEEP(interfaz, unidadesDeTrabajo);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_STDIN_READ") == 0) {

        char* interfaz = instruccion->param1;
        char* registroDireccion = instruccion->param2;
		char* registroTamanio = instruccion->param3;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_STDIN_READ> - <%d> - <%s> - <%s>", PCB_Actual->id, interfaz, registroDireccion, registroTamanio);

		//Esta instrucción solicita al Kernel que mediante la interfaz ingresada se lea desde el STDIN (Teclado) un valor cuyo tamaño está delimitado por el valor del Registro Tamaño y el mismo se guarde a partir de la Dirección Lógica almacenada en el Registro Dirección.
        IO_STDIN_READ(interfaz, registroDireccion, registroTamanio);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_STDOUT_WRITE") == 0) {

        char* interfaz = instruccion->param1;
        char* registroDireccion = instruccion->param2;
		char* registroTamanio = instruccion->param3;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_STDOUT_WRITE> - <%d> - <%s> - <%s>", PCB_Actual->id, interfaz, registroDireccion, registroTamanio);

		//Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde la posición de memoria indicada por la Dirección Lógica almacenada en el Registro Dirección, un tamaño indicado por el Registro Tamaño y se imprima por pantalla.
        IO_STDOUT_WRITE(interfaz, registroDireccion, registroTamanio);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_FS_CREATE") == 0) {

        char* interfaz = instruccion->param1;
        char* nombreArchivo = instruccion->param2;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_FS_CREATE> - <%d> - <%s>", PCB_Actual->id, interfaz, nombreArchivo);

		//Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se cree un archivo en el FS montado en dicha interfaz.
        IO_FS_CREATE(interfaz, nombreArchivo);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_FS_DELETE") == 0) {

        char* interfaz = instruccion->param1;
        char* nombreArchivo = instruccion->param2;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_FS_DELETE> - <%d> - <%s>", PCB_Actual->id, interfaz, nombreArchivo);

		//Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se elimine un archivo en el FS montado en dicha interfaz
        IO_FS_DELETE(interfaz, nombreArchivo);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_FS_TRUNCATE") == 0) {

        char* interfaz = instruccion->param1;
        char* nombreArchivo = instruccion->param2;
		char* registroTamanio = instruccion->param3;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_FS_TRUNCATE> - <%d> - <%s> - <%s>", PCB_Actual->id, interfaz, nombreArchivo, registroTamanio);

		//Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se modifique el tamaño del archivo en el FS montado en dicha interfaz, actualizando al valor que se encuentra en el registro indicado por Registro Tamaño.
        IO_FS_TRUNCATE(interfaz, nombreArchivo, registroTamanio);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_FS_WRITE") == 0) {

        char* interfaz = instruccion->param1;
        char* nombreArchivo = instruccion->param2;
		char* registroDireccion = instruccion->param3;
		char* registroTamanio = instruccion->param4;
		char* registroPunteroArchivo = instruccion->param5;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_FS_WRITE> - <%d> - <%s> - <%s> - <%s> - <%s>", PCB_Actual->id, interfaz, nombreArchivo, registroDireccion, registroTamanio, registroPunteroArchivo);

		//Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde Memoria la cantidad de bytes indicadas por el Registro Tamaño a partir de la dirección lógica que se encuentra en el Registro Dirección y se escriban en el archivo a partir del valor del Registro Puntero Archivo.
        IO_FS_WRITE(interfaz, nombreArchivo, registroDireccion, registroTamanio, registroPunteroArchivo);
    }

	else if (strcmp(nombre_instruccion_actual, "IO_FS_READ") == 0) {

        char* interfaz = instruccion->param1;
        char* nombreArchivo = instruccion->param2;
		char* registroDireccion = instruccion->param3;
		char* registroTamanio = instruccion->param4;
		char* registroPunteroArchivo = instruccion->param5;

        log_info(info_logger, "PID: <%d> - Ejecutando: <IO_FS_READ> - <%d> - <%s> - <%s> - <%s> - <%s>", PCB_Actual->id, interfaz, nombreArchivo, registroDireccion, registroTamanio, registroPunteroArchivo);

		//Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde el archivo a partir del valor del Registro Puntero Archivo la cantidad de bytes indicada por Registro Tamaño y se escriban en la Memoria a partir de la dirección lógica indicada en el Registro Dirección.
        IO_FS_READ(interfaz, nombreArchivo, registroDireccion, registroTamanio, registroPunteroArchivo);
    }

    else if (strcmp(nombre_instruccion_actual, "EXIT") == 0) {
        log_info(info_logger, "PID: <%d> - Ejecutando: <EXIT> -", PCB_Actual->id);

        ejecutar_EXIT();
    } else {
        log_info(info_logger, "PID: <%d> - NO EXISTE LA INSTRUCCION %s", PCB_Actual->id, nombre_instruccion_actual); // log_error() ?

    }
    //liberarInstruccion(instruccion);
}

void checkInsterrupt(){

}

void copiar_registroPCB_a_los_registrosCPU (RegistrosCPU* registro) {
	memcpy(registroCPU_AX,registro->registro_AX,1);
	memcpy(registroCPU_BX,registro->registro_BX,1);
	memcpy(registroCPU_CX,registro->registro_CX,1);
	memcpy(registroCPU_DX,registro->registro_DX,1);
    memcpy(registroCPU_EAX,registro->registro_EAX,4);
	memcpy(registroCPU_EBX,registro->registro_EBX,4);
	memcpy(registroCPU_ECX,registro->registro_ECX,4);
	memcpy(registroCPU_EDX,registro->registro_EDX,4);
    memcpy(registroCPU_SI,registro->registro_SI,4);
	memcpy(registroCPU_DI,registro->registro_DI,4);
}

void copiar_registrosCPU_a_los_registroPCB(RegistrosCPU* registro) {
    memcpy(registro->registro_AX,registroCPU_AX,1);
    memcpy(registro->registro_BX,registroCPU_BX,1);
    memcpy(registro->registro_CX,registroCPU_CX,1);
    memcpy(registro->registro_DX,registroCPU_DX,1);
    memcpy(registro->registro_EAX,registroCPU_EAX,4);
    memcpy(registro->registro_EBX,registroCPU_EBX,4);
    memcpy(registro->registro_ECX,registroCPU_ECX,4);
    memcpy(registro->registro_EDX,registroCPU_EDX,4);
    memcpy(registro->registro_SI,registroCPU_SI,4);
    memcpy(registro->registro_DI,registroCPU_DI,4);
}
#include "logs.h"


t_log* info_logger;
t_log* trace_logger;
t_log* error_logger;
bool logsCreados = false;


int init_loggers(){

    trace_logger = log_create("trace_logger.log", "entradaSalida", true, LOG_LEVEL_TRACE);
    info_logger = log_create("info_logger.log", "entradaSalida", true, LOG_LEVEL_INFO);
    error_logger = log_create("error_logger.log","entradaSalida", true, LOG_LEVEL_ERROR);

     if(trace_logger== NULL || info_logger== NULL || error_logger== NULL){

        printf("No pude crear los loggers");
        return false;
    }

    logsCreados = true;

    return true;                //TODO: CUANDO ESTÉN LAS CONFIGS, CAMBIAR. 
}

void logOperacion(uint32_t pid, char* operacionARealizar){ 
    log_info(info_logger, "PID: <%d> - Operacion: <%s>", pid, operacionARealizar);
}

void logCrearArchivo(uint32_t pid, char* nombreArchivo){
    log_info(info_logger, "PID: <%d> - Crear Archivo: <%s>", pid, nombreArchivo);
}

void logTruncarArchivo(uint32_t pid, char* nombreArchivo){
    log_info(info_logger, "PID: <%d> - Truncar Archivo: <%s>", pid, nombreArchivo);
}

void logEliminarArchivo(uint32_t pid, char* nombreArchivo, uint32_t tamArchivoTruncado){
    log_info(info_logger, "PID: <%d> - Eliminar Archivo: <%s> - Tamaño <%d>", pid, nombreArchivo, tamArchivoTruncado);
}

void logLeerArchivo(uint32_t pid, char* nombreArchivo, uint32_t tamanioAEscribir, uint32_t punteroArchivo){
    log_info(info_logger, "PID: <%d> - Leer Archivo: <%s> - Tamaño a Leer: <%d> - Puntero Archivo: <%d>", pid, nombreArchivo, tamanioAEscribir, punteroArchivo);
}

void logEscribirArchivo(uint32_t pid, char* nombreArchivo, uint32_t tamanioAEscribir, uint32_t punteroArchivo){
    log_info(info_logger, "PID: <%d> - Escribir Archivo: <%s> - Tamaño a escribir: <%d> - Puntero Archivo: <%d>", pid, nombreArchivo, tamanioAEscribir, punteroArchivo);
}
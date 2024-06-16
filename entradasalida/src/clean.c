#include <clean.h>

void cerrarPrograma(){

    log_trace(trace_logger, "Cerrando programa...");

    if(logsCreados){
        destruirLoggers();
    }
    if(strcmp("DIALFS", cfg_entradaSalida->TIPO_INTERFAZ) == 0){
        close(bitmap->fd);
        free(bitmap);
    }
}

void destruirLoggers(){
    log_destroy(info_logger);
    log_destroy(error_logger);
    log_destroy(trace_logger);
    log_destroy(debug_logger);

}
#include <logs.h>


t_log* info_logger;
t_log* trace_logger;
t_log* error_logger;
t_config* configuracionEntradasalida;
bool logsCreados = false;
bool configCreada = false;
t_config_entradaSalida* cfg_entradaSalida;
archBloques* archivoBloques;
uint32_t* bitmap;
t_list* listaArchivos;

void crearSemaforos() {
    pthread_mutex_init(&mutex_recvKernel, NULL);
    pthread_mutex_init(&mutex_recvMemoria, NULL);
    
    pthread_mutex_init(&mutex_peticiones_pendientes, NULL);

    sem_init(&sem_contador_peticiones, 0, 0);
}

void crearListas() {
   lista_peticiones_pendientes = list_create();
}

void crearEstructurasFs() {

    crearBitmap();

    crearArchivoBloques();

    listaArchivos = list_create();
}

void crearBitmap() {

   int cantidadDeBloques =  cfg_entradaSalida->BLOCK_COUNT;
    bitmap = (uint32_t *)malloc(cantidadDeBloques * sizeof(uint32_t));

    for(int i=0; i<cantidadDeBloques; i++){
        bitmap[i] = 0;
    }
    log_trace(trace_logger, "Bitmap creado");
}



void crearArchivoBloques() {
    
    archivoBloques = malloc(sizeof(archBloques));
    int blockCount = cfg_entradaSalida->BLOCK_COUNT;
    int blockSize = cfg_entradaSalida->BLOCK_SIZE;
    archivoBloques->tamanio = blockCount*blockSize;

    char* path = string_new();
    string_append(&path, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&path, "/bloques.dat");
    

    log_info(info_logger, "%s", path);
    
    archivoBloques->fd = open(path, O_CREAT| O_RDWR,  S_IRUSR|S_IWUSR);
    if (archivoBloques->fd == -1){
        log_trace(trace_logger,"Error al abrir/crear el archivo de bloques");
        return;
    }

    ftruncate(archivoBloques->fd, archivoBloques->tamanio);
    archivoBloques->direccionArchivo = mmap(NULL, archivoBloques->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, archivoBloques->fd , 0);

    if(archivoBloques->direccionArchivo == MAP_FAILED) {
	       log_trace(trace_logger, "Error al mapear el archivo en memoria");
	       close(archivoBloques->fd);
	       return;
	    }

    msync(archivoBloques->direccionArchivo, archivoBloques->tamanio, MS_SYNC);

    log_trace(trace_logger, "Archivo de Bloques creado");
    close(archivoBloques->fd);
}



int init_loggers_config(char* path){

    trace_logger = log_create("trace_logger.log", "entradaSalida", true, LOG_LEVEL_TRACE);
    info_logger = log_create("info_logger.log", "entradaSalida", true, LOG_LEVEL_INFO);
    error_logger = log_create("error_logger.log","entradaSalida", true, LOG_LEVEL_ERROR);

    if(trace_logger== NULL || info_logger== NULL || error_logger== NULL){

        printf("No pude crear los loggers");
        return false;
    }

    cfg_entradaSalida = cfg_entradaSalida_start();

    if(cfg_entradaSalida == NULL){
        return false;
    }
    configCreada = true;
    logsCreados = true;

    configuracionEntradasalida = config_create(path);
    if(configuracionEntradasalida == NULL){
        printf("no pude leer la config");
    }

    return true;                //TODO: CUANDO ESTÉN LAS CONFIGS, CAMBIAR. 
}

t_config_entradaSalida *cfg_entradaSalida_start()
{
    t_config_entradaSalida *cfg = malloc(sizeof(t_config_entradaSalida));
    return cfg;
}

int cargar_configuracion(){
    cfg_entradaSalida->IP_KERNEL = config_get_string_value(configuracionEntradasalida, "IP_KERNEL");
    log_trace(trace_logger, "IP_KERNEL Cargada Correctamente: %s", cfg_entradaSalida->IP_KERNEL);

    cfg_entradaSalida->IP_MEMORIA = config_get_string_value(configuracionEntradasalida, "IP_MEMORIA");
    log_trace(trace_logger, "IP_MEMORIA Cargada Correctamente: %s", cfg_entradaSalida->IP_MEMORIA);

    cfg_entradaSalida->PUERTO_KERNEL = config_get_string_value(configuracionEntradasalida, "PUERTO_KERNEL");
    log_trace(trace_logger, "PUERTO_KERNEL Cargado Correctamente: %s", cfg_entradaSalida->PUERTO_KERNEL);

    cfg_entradaSalida->PUERTO_MEMORIA = config_get_string_value(configuracionEntradasalida, "PUERTO_MEMORIA");
    log_trace(trace_logger, "PUERTO_MEMORIA Cargado Correctamente: %s", cfg_entradaSalida->PUERTO_MEMORIA);

    cfg_entradaSalida->BLOCK_COUNT = config_get_int_value(configuracionEntradasalida, "BLOCK_COUNT");
    log_trace(trace_logger, "BLOCK_COUNT Cargado Correctamente: %d", cfg_entradaSalida->BLOCK_COUNT);

    cfg_entradaSalida->BLOCK_SIZE = config_get_int_value(configuracionEntradasalida, "BLOCK_SIZE");
    log_trace(trace_logger, "BLOCK_SIZE Cargado Correctamente: %d", cfg_entradaSalida->BLOCK_SIZE);

    cfg_entradaSalida->PATH_BASE_DIALFS = config_get_string_value(configuracionEntradasalida, "PATH_BASE_DIALFS");
    log_trace(trace_logger, "PATH_BASE_DIALFS Cargado Correctamente: %s", cfg_entradaSalida->PATH_BASE_DIALFS);

    cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO = config_get_int_value(configuracionEntradasalida, "TIEMPO_UNIDAD_TRABAJO");
    log_trace(trace_logger, "TIEMPO_UNIDAD_TRABAJO Cargado Correctamente: %d", cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO);

    cfg_entradaSalida->TIPO_INTERFAZ = config_get_string_value(configuracionEntradasalida, "TIPO_INTERFAZ");
    log_trace(trace_logger, "TIPO_INTERFAZ Cargada Correctamente: %s", cfg_entradaSalida->TIPO_INTERFAZ);

    cfg_entradaSalida->RETRASO_COMPACTACION = config_get_int_value(configuracionEntradasalida, "RETRASO_COMPACTACION");
    log_trace(trace_logger, "RETRASO_COMPACTACION Cargado Correctamente: %d", cfg_entradaSalida->RETRASO_COMPACTACION);

    if(strcmp(cfg_entradaSalida->TIPO_INTERFAZ, "STDOUT") == 0)
		cfg_entradaSalida->TIPO_INTERFAZ_INT = 0;
	else if(strcmp(cfg_entradaSalida->TIPO_INTERFAZ, "STDIN") == 0)
		cfg_entradaSalida->TIPO_INTERFAZ_INT = 1;
	else if(strcmp(cfg_entradaSalida->TIPO_INTERFAZ, "DIAL_FS") == 0)
		cfg_entradaSalida->TIPO_INTERFAZ_INT = 2;
	else
		cfg_entradaSalida->TIPO_INTERFAZ_INT = 3;

    return true;
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




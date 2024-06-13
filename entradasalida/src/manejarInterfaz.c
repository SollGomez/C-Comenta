#include <manejarInterfaz.h>


void cualInterfaz() {

    switch (cfg_entradaSalida->TIPO_INTERFAZ_INT)
    {
    case 0:  //STDOUT
        /* code */
        break;
    case 1:  //STDIN
        /* code */
        break;
    case 2:  //DIALFS
        /* code */
        break;
    case 3:  //GENERICA
        break;
    default:
        printf("Esa interfaz no existe :/");
        break;
    }

    return;
}


void manejarInterfazGenerica(uint32_t unidadesDeTrabajo) {
    
    
    
        sleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO+4);               //DESPUES VER SI ES USLEEP O SLEEP
        // usleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO * 100000);
        printf("SLEEP TERMINADO\n");
}

void manejarInterfazStdin(uint32_t direccionFisicaAEscribir) {
    char* input;
    input = readline(">");

    log_trace(trace_logger, "El texto ingresado fue: %s", input);
    enviarEnteroYString(direccionFisicaAEscribir, input, memoria_fd, info_logger, IO_STDIN_READ_DONE);

}


// FileSystem
void crearArchivo(char* nombreArchivo) {

    char* base = string_new();
    string_append(&base, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&base, "/");
    string_append(&base, nombreArchivo); // se da por hecho que esta el .txt
    //string_append(base, ".txt");

    log_info(info_logger, "%s", base);
    FILE* f = fopen(base, "rb");
    if(f != NULL) {
        log_info(info_logger, "El archivo ya existe");
        fclose(f);
        return;
    }

    f = fopen(base, "wb");
    if(f == NULL) {
        log_info(info_logger, "No se creo el archivo");
    }
  
    t_archivo_metadata* archivoCreado = malloc(sizeof(t_archivo_metadata));
    
    int tamanio_nombre_archivo = strlen(nombreArchivo);

    archivoCreado->nombreArchivo = malloc(sizeof(tamanio_nombre_archivo + 1));
    archivoCreado->tamArchivo = 0;
    strcpy(archivoCreado->nombreArchivo, nombreArchivo);

    t_config* configAux = config_create(base);

    
    archivoCreado->bloqueInicial = obtener_bloque_libre(bitmap);
    if(archivoCreado->bloqueInicial == -1) {
        return;
    }

    char* stringAuxiliar = string_itoa(archivoCreado->bloqueInicial);
    
    config_set_value(configAux, "NOMBRE_ARCHIVO", nombreArchivo);
    config_set_value(configAux, "TAMANIO_ARCHIVO", "0");
    config_set_value(configAux, "BLOQUE_INICIAL", stringAuxiliar);
	config_save(configAux);

    archivoCreado->configArchivo = configAux;
    fclose(f);
}

uint32_t obtener_bloque_libre (t_bitarray* bitmap) {

    for (uint32_t i = 0 ; i < bitarray_get_max_bit(bitmap); i++) {
            //accesoABitmap(i, bitarray_test_bit(bitmap, i));

        if (!bitarray_test_bit(bitmap, i)) {

            bitarray_set_bit(bitmap, i);
            //accesoABitmap(i, bitarray_test_bit(bitmap, i));

            return i;
        }
    }

    msync(bitmap, cfg_entradaSalida->BLOCK_COUNT/8, MS_SYNC);
	log_info(info_logger, "No se obtuvo un bloque libre");
    return -1;
}

void eliminarArchivo(char* nombreArchivo) {
    char* base = string_new();
    string_append(&base, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&base, "/");
    string_append(&base, nombreArchivo); // se da por hecho que esta el .txt
    //string_append(base, ".txt");

    if (remove(base) == 0) {
        log_info(info_logger, "El archivo %s ha sido borrado exitosamente.\n", base);
    } else {
        log_info(info_logger, "Error al borrar el archivo");
    }
}

void truncarArchivo (char* nombreArchivo, uint32_t tamanio) {

    log_trace(trace_logger, "Entre a truncar");
    char* base = string_new();
    string_append(&base, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&base, "/");
    string_append(&base, nombreArchivo); // se da por hecho que esta el .txt
    //string_append(base, ".txt");
    FILE* f = fopen(base, "ab");

    t_archivo_metadata* archivoATruncar = malloc(sizeof(t_archivo_metadata));
    archivoATruncar->configArchivo = config_create(base);
    archivoATruncar->bloqueInicial = config_get_int_value(archivoATruncar->configArchivo, "BLOQUE_INICIAL");
    archivoATruncar->tamArchivo = config_get_int_value(archivoATruncar->configArchivo, "TAMANIO_ARCHIVO");

    if(archivoATruncar->tamArchivo == tamanio)
        return;

    if(archivoATruncar->tamArchivo > tamanio){

        if(tamanio < 0) {
            log_info(info_logger, "No puede haber tamanio negativo");
            return;
        }

		achicarArchivo(nombreArchivo, tamanio, archivoATruncar);
        log_trace(trace_logger, "Nuevo tamanio del archivo %s achicado: %d", nombreArchivo, tamanio);

	}else{
        agrandarArchivo(nombreArchivo, tamanio, archivoATruncar);
        log_trace(trace_logger, "Nuevo tamanio del archivo %s agrandado: %d", nombreArchivo, tamanio);
	}

    //msync(bitmap, cfg_entradaSalida->BLOCK_COUNT/8, MS_SYNC);
    char* stringAuxiliar = string_itoa(tamanio);
    config_set_value(archivoATruncar->configArchivo, "TAMANIO_ARCHIVO", stringAuxiliar);
    config_save(archivoATruncar->configArchivo);

    fclose(f);
}

void agrandarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar) {

   // uint32_t bloquesActuales = archivoATruncar->tamArchivo / cfg_entradaSalida->BLOCK_SIZE; 

    uint32_t bloquesASumar = (tamanio - archivoATruncar->tamArchivo) / cfg_entradaSalida->BLOCK_SIZE;
    uint32_t tamanioActual = archivoATruncar->tamArchivo / cfg_entradaSalida->BLOCK_SIZE;
    uint32_t posicionFinalActual;
     
    if(!tamanioActual) {
        posicionFinalActual = archivoATruncar->bloqueInicial;
    }else {
        posicionFinalActual = archivoATruncar->bloqueInicial + tamanioActual - 1;
    }
    
    //uint32_t posicionFinalActual = archivoATruncar->bloqueInicial + tamanioActual - 1;
    //ROMPE :( revisar
    if (tengoEspacioAMiLado(archivoATruncar, tamanio)) {              // AGRANDAMOS NORMAL
        log_info(info_logger, "Tengo espacio a mi lado");
        while(bloquesASumar) {

            bitarray_set_bit(bitmap, posicionFinalActual + bloquesASumar);
            bloquesASumar--;
        }
        return;
    }

    uint32_t posInicialNueva = hayEspacioContiguo(tamanio / cfg_entradaSalida->BLOCK_SIZE);

    if(posInicialNueva != -1){
        log_info(info_logger, "Hay espacio sin compactar, posicion inicial nueva del achivo %s: %d", nombreArchivo, posInicialNueva);

        for(int i=archivoATruncar->bloqueInicial; i<posicionFinalActual; i++){
            log_info(info_logger, "entre al for con inicial %d y final %d", archivoATruncar->bloqueInicial, posicionFinalActual);
            bitarray_clean_bit(bitmap, i);
        }

        char* stringAuxiliar = string_itoa(posInicialNueva);
        config_set_value(archivoATruncar->configArchivo, "BLOQUE_INICIAL", stringAuxiliar);
        config_save(archivoATruncar->configArchivo);

        uint32_t posicionFinal = posInicialNueva + bloquesASumar;

        for(int i=posInicialNueva; i<posicionFinal; i++){
            bitarray_set_bit(bitmap, i);
        }
        return;
    }

    //TODO COMPACTACION :'(
    
    
    return;

}

bool tengoEspacioAMiLado(t_archivo_metadata* archivoATruncar, uint32_t tamanioNuevo) {

    uint32_t tamanioNuevoEnBloques = tamanioNuevo / cfg_entradaSalida->BLOCK_SIZE;

    uint32_t tamanioActual = archivoATruncar->tamArchivo / cfg_entradaSalida->BLOCK_SIZE;

    uint32_t bloquesASumar = tamanioNuevoEnBloques - tamanioActual;

    uint32_t posicionFinalActual;
     
    if(!tamanioActual) {
        posicionFinalActual = archivoATruncar->bloqueInicial;
    }else {
        posicionFinalActual = archivoATruncar->bloqueInicial + tamanioActual - 1;
    }

    uint32_t posicionFinal = posicionFinalActual + bloquesASumar;

    for(uint32_t i = posicionFinalActual + 1; i < posicionFinal; i++) {

        if(bitarray_test_bit(bitmap, i)) {
            return false;
        }
    }

    return true;
}

void achicarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar) {
    
    uint32_t bloquesActuales = archivoATruncar->tamArchivo / cfg_entradaSalida->BLOCK_SIZE; 

    uint32_t bloquesARestar = (archivoATruncar->tamArchivo - tamanio) / cfg_entradaSalida->BLOCK_SIZE;
    
    log_info(info_logger, "Bloque restado: %d", bloquesActuales);

    while(bloquesARestar) {

        bloquesARestar--;
        if(bloquesARestar == 0)
            break;

        bitarray_clean_bit(bitmap, bloquesActuales - 1);
        bloquesActuales--;
    }

    return; 
}

uint32_t hayEspacioContiguo(uint32_t blocksRequested) {
    bool esElPrimero = true;
    uint32_t bloquesLibres = 0;
    uint32_t bloqueInicialNuevo = 0;
    log_info(info_logger, "blocks requested: %d", blocksRequested);
    for(int i=0; i<bitarray_get_max_bit(bitmap); i++) {

        if(!bitarray_test_bit(bitmap, i)){
            if(esElPrimero) {
                bloqueInicialNuevo = i;
            }
            esElPrimero = false;
            bloquesLibres ++;
            log_info(info_logger, "bloques pasados: %d", i);
            if(bloquesLibres == blocksRequested)
                return bloqueInicialNuevo;

        }else{
            esElPrimero = true;
            bloquesLibres = 0;
        }
    }
    return -1;
}

void escribirArchivo(char* nombreArchivo, void* datos, uint32_t direccionAEscribir, uint32_t tamanioAEscribir) {
    //uint32_t offset = direccionAEscribir % cfg_entradaSalida->BLOCK_SIZE;
    //log_info(info_logger, "Offset: %d", offset);
    //uint32_t numeroDeBloque = encontrarNumeroBloque(direccionAEscribir); //no se usa porque damos por hecho que la dir es pasada en bytes    

    log_info(info_logger, "Escribo esta data: %s", datos);

    char* pathArchivoBloques = string_new();

    string_append(&pathArchivoBloques, cfg_entradaSalida->PATH_BASE_DIALFS);

    string_append(&pathArchivoBloques, "/bloques.dat");

    archivoBloques->fd = open(pathArchivoBloques, O_CREAT| O_RDWR,  S_IRUSR|S_IWUSR);

    archivoBloques->direccionArchivo = mmap(NULL,archivoBloques->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, archivoBloques->fd , 0);

    memcpy(archivoBloques->direccionArchivo + direccionAEscribir, datos, tamanioAEscribir);
    
    msync(archivoBloques->direccionArchivo, archivoBloques->tamanio, MS_SYNC);

    close(archivoBloques->fd);
    
}

//no se usa porque damos por hecho que la dir es pasada en bytes
uint32_t encontrarNumeroBloque(uint32_t direccion) {
    return direccion / cfg_entradaSalida->BLOCK_SIZE;
}

void* leerArchivo(char* nombreArchivo, uint32_t direccionALeer, uint32_t tamanioALeer) {

    void* datos = malloc(tamanioALeer);

    char* pathArchivoBloques = string_new();

    string_append(&pathArchivoBloques, cfg_entradaSalida->PATH_BASE_DIALFS);

    string_append(&pathArchivoBloques, "/bloques.dat");

    archivoBloques->fd = open(pathArchivoBloques, O_CREAT| O_RDWR,  S_IRUSR|S_IWUSR);

    archivoBloques->direccionArchivo = mmap(NULL,archivoBloques->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, archivoBloques->fd , 0);

    //msync(archivoBloques->archivo, archivoBloques->tamanio, MS_SYNC);
    memcpy(datos, archivoBloques->direccionArchivo + direccionALeer, tamanioALeer);

    log_info(info_logger, "La data que lei es: %s", datos);

    close(archivoBloques->fd);

    return datos;
}
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

void mostrarBitmap() {
	for(int i=0; i<bitarray_get_max_bit(bitmap); i++){
		log_info(info_logger, "bit en la posicion %d es: %d", i, bitarray_test_bit(bitmap, i));
		if(i == 10)
			break;
	}
}


void manejarInterfazGenerica(uint32_t unidadesDeTrabajo) {    
        sleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO+4);               //DESPUES VER SI ES USLEEP O SLEEP
        // usleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO * 100000);
        printf("SLEEP TERMINADO\n");
}

void manejarInterfazStdin(uint32_t direccionFisicaAEscribir, uint32_t pid) {

    t_datos* datos = malloc(sizeof(t_datos));
    datos->datos = readline(">");

    t_list* listaEnteros;
    listaEnteros = list_create();

    list_add(listaEnteros, &pid);
    list_add(listaEnteros, &direccionFisicaAEscribir);

    log_trace(trace_logger, "El texto ingresado fue: %s", datos->datos);
    
    enviarListaIntsYDatos(listaEnteros, datos, memoria_fd, info_logger, ACCESO_PEDIDO_ESCRITURA);


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

void truncarArchivo(char* nombreArchivo, uint32_t tamanio, uint32_t pid) {
    log_trace(trace_logger, "Entre a truncar");
    char* base = string_new();
    string_append(&base, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&base, "/");
    string_append(&base, nombreArchivo);

    t_archivo_metadata* archivoATruncar = malloc(sizeof(t_archivo_metadata));
    archivoATruncar->configArchivo = config_create(base);
    if (!archivoATruncar->configArchivo) {
        log_error(error_logger, "No se pudo crear la configuración del archivo %s", base);
        free(base);
        free(archivoATruncar);
        return;
    }
    archivoATruncar->bloqueInicial = config_get_int_value(archivoATruncar->configArchivo, "BLOQUE_INICIAL");
    archivoATruncar->tamArchivo = config_get_int_value(archivoATruncar->configArchivo, "TAMANIO_ARCHIVO");

    if (archivoATruncar->tamArchivo == tamanio) {
        config_destroy(archivoATruncar->configArchivo);
        free(archivoATruncar);
        free(base);
        return;
    }

    if (archivoATruncar->tamArchivo > tamanio) {
        if (tamanio < 0) {
            log_info(info_logger, "No puede haber tamanio negativo");
            config_destroy(archivoATruncar->configArchivo);
            free(archivoATruncar);
            free(base);
            return;
        }
        achicarArchivo(nombreArchivo, tamanio, archivoATruncar);
    } else {
        agrandarArchivo(nombreArchivo, tamanio, archivoATruncar, pid);
    }

    char* stringAuxiliar = string_itoa(tamanio);
    config_set_value(archivoATruncar->configArchivo, "TAMANIO_ARCHIVO", stringAuxiliar);
    config_save(archivoATruncar->configArchivo);
    free(stringAuxiliar);

    config_destroy(archivoATruncar->configArchivo);
    free(archivoATruncar);
    free(base);
}


void agrandarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar, uint32_t pid) {

    uint32_t bloquesASumar = (tamanio + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE - 
                             (archivoATruncar->tamArchivo + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE;
    uint32_t tamanioActual = (archivoATruncar->tamArchivo + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE;   
    uint32_t posicionFinalActual = (tamanioActual == 0) ? archivoATruncar->bloqueInicial : archivoATruncar->bloqueInicial + tamanioActual - 1;

    if (tengoEspacioAMiLado(archivoATruncar, tamanio)) {
        log_info(info_logger, "Tengo espacio a mi lado");
        uint32_t posicionInicialAsignacion = (tamanioActual == 0) ? archivoATruncar->bloqueInicial : posicionFinalActual + 1;
        //log_debug(debug_logger, "Posicion inicial de asignación <%d> bloques a sumar <%d>", posicionInicialAsignacion, bloquesASumar);
        while (bloquesASumar > 0) {
            bitarray_set_bit(bitmap, posicionInicialAsignacion);
            log_info(info_logger, "Asigno bit: %d", posicionInicialAsignacion);
            posicionInicialAsignacion++;
            bloquesASumar--;
        }
    } else {
        compactar(nombreArchivo, tamanio, archivoATruncar, pid);
    }
}


bool hayBloquesAtrasDelArchivoATruncar(t_archivo_metadata* archivoATruncar, uint32_t* bloquesLibresDetras) {

    log_info(info_logger,  "hay bloques libres detras?");

    *bloquesLibresDetras = 0;
    uint32_t bloqueInicialArchivo = archivoATruncar->bloqueInicial;
    
    if (bloqueInicialArchivo == 0) return false; // Si está en el primer bloque, no hay bloques detrás
    
    uint32_t bloqueActual = bloqueInicialArchivo - 1;
    
    while (bloqueActual >= 0 && !bitarray_test_bit(bitmap, bloqueActual)) {
        (*bloquesLibresDetras)++;
        if (bloqueActual == 0) break; // Para evitar underflow
        bloqueActual--;
    }
    
    return (*bloquesLibresDetras > 0);
}

void moverArchivosHaciaAtras(char* nombreArchivo, uint32_t bloquesLibresDetras, t_archivo_metadata* archivoATruncar) {
    log_info(info_logger, "Muevo archivos hacia atras %d bloques", bloquesLibresDetras);

    int archivoObjetivoIndex = -1;
    for (int i = 0; i < list_size(listaDeArchivos); i++) {
        if (strcmp(nombreArchivo, list_get(listaDeArchivos, i)) == 0) {
            archivoObjetivoIndex = i;
            break;
        }
    }

    if (archivoObjetivoIndex == -1) {
        log_error(error_logger, "Archivo objetivo no encontrado en la lista: %s", nombreArchivo);
        return;
    }

    for (int i = archivoObjetivoIndex; i < list_size(listaDeArchivos); i++) {
        char* nombreArchivoActual = list_get(listaDeArchivos, i);
        char* path = string_new();
        string_append(&path, cfg_entradaSalida->PATH_BASE_DIALFS);
        string_append(&path, "/");
        string_append(&path, nombreArchivoActual);

        t_config* config = config_create(path);
        if (!config) {
            log_error(error_logger, "No se pudo crear la configuración del archivo %s", path);
            free(path);
            continue;
        }

        log_trace(trace_logger, "Moviendo archivo <%s> hacia atrás", nombreArchivoActual);
        uint32_t bloqueInicialArchivoAMover = config_get_int_value(config, "BLOQUE_INICIAL");
        uint32_t tamArchivoAMover = config_get_int_value(config, "TAMANIO_ARCHIVO");

        uint32_t posicionFinalArchivoAMover = (tamArchivoAMover == 0) ? bloqueInicialArchivoAMover : 
                                              bloqueInicialArchivoAMover + (tamArchivoAMover + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE - 1;

        if (bloqueInicialArchivoAMover < bloquesLibresDetras) {
            log_error(error_logger, "No se puede mover el archivo %s porque resulta en una posición negativa", nombreArchivoActual);
            free(path);
            config_destroy(config);
            continue;
        }

        for (uint32_t bloque = bloqueInicialArchivoAMover; bloque <= posicionFinalArchivoAMover; bloque++) {
            bitarray_clean_bit(bitmap, bloque);
        }

        uint32_t nuevaPosicionInicial = bloqueInicialArchivoAMover - bloquesLibresDetras;
        uint32_t nuevaPosicionFinal = posicionFinalArchivoAMover - bloquesLibresDetras;
        for (uint32_t bloque = nuevaPosicionInicial; bloque <= nuevaPosicionFinal; bloque++) {
            log_info(info_logger, "Me seteo en %d", bloque);
            bitarray_set_bit(bitmap, bloque);
        }

        void* datosAMover = leerArchivo(nombreArchivoActual, bloqueInicialArchivoAMover, tamArchivoAMover);
        escribirArchivo(nombreArchivoActual, datosAMover, nuevaPosicionInicial, tamArchivoAMover);

        char* stringAux = string_itoa(nuevaPosicionInicial);
        config_set_value(config, "BLOQUE_INICIAL", stringAux);
        config_save(config);
        free(stringAux);

        mostrarBitmap();

        free(path);
        config_destroy(config);
    }
}




void compactar(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar, uint32_t pid) {
    
    logInicioCompactacion(pid);

    log_trace(trace_logger, "Estoy truncando el archivo <%s>", nombreArchivo);

    uint32_t bloquesLibresDetras = 0;
    if (hayBloquesAtrasDelArchivoATruncar(archivoATruncar, &bloquesLibresDetras)) {
        moverArchivosHaciaAtras(nombreArchivo, bloquesLibresDetras, archivoATruncar);
    }

    char* path2 = string_new();
    string_append(&path2, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&path2, "/");
    string_append(&path2, nombreArchivo);
    
    FILE* f2 = fopen(path2, "ab");
    if(f2 == NULL) {
        log_info(info_logger, "No se pudo abrir el archivo %s", path2);
        free(path2);
        //continue;
    }


    t_config* config2 = config_create(path2);

    uint32_t bloqueAHORA = config_get_int_value(config2, "BLOQUE_INICIAL");
    archivoATruncar->bloqueInicial = bloqueAHORA;
    //log_debug(debug_logger, "Bloque inicial del archivo <%s> a la hora de compactar <%d>", nombreArchivo, bloqueAHORA);
    free(path2);

    uint32_t bloquesAMoverse = (tamanio / cfg_entradaSalida->BLOCK_SIZE) - 1; //La cantidad de bloques que todos los archivos se van a mover para darle espacio a este archivo

    // Se da por hecho que los archivos de la lista están ordenados de primero a ultimo (Debería tener sentido)
    for(int i=list_size(listaDeArchivos) - 1; i>=0; i--) {


        if(strcmp(nombreArchivo, list_get(listaDeArchivos, i)) == 0){
            break;    //Sale porque en el orden de los archivos este sería al que hay que ampliar, entonces no se debería contar ni mover
        }

        char* path = string_new();
        string_append(&path, cfg_entradaSalida->PATH_BASE_DIALFS);
        string_append(&path, "/");
        string_append(&path, list_get(listaDeArchivos, i));

        //log_debug(debug_logger, "El archivo <%s> está siendo movido así puedo truncar", path);

        FILE* f = fopen(path, "ab");
        if(f == NULL) {
            log_info(info_logger, "No se pudo abrir el archivo %s", path);
            free(path);
            continue;
        }


        t_config* config = config_create(path);
        uint32_t bloqueInicialArchivoAMover = 0;
        bloqueInicialArchivoAMover = config_get_int_value(config, "BLOQUE_INICIAL");
        uint32_t tamArchivoAMover = config_get_int_value(config, "TAMANIO_ARCHIVO");
        
        uint32_t posicionFinalArchivoAMover = (tamArchivoAMover == 0) ? bloqueInicialArchivoAMover : 
                                              bloqueInicialArchivoAMover + (tamArchivoAMover + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE - 1;
        // if(!tamArchivoAMover){
        //     posicionFinalArchivoAMover = bloqueInicialArchivoAMover;
        // } else {
        //     posicionFinalArchivoAMover = bloqueInicialArchivoAMover + (tamArchivoAMover/cfg_entradaSalida->BLOCK_SIZE) - 1;
        // }

        log_trace(trace_logger, "Datos: posicionfinalArchivo: %d, bloqueInicialarchivoAMover: %d, cantidad de bloques a moverse: %d", posicionFinalArchivoAMover, bloqueInicialArchivoAMover, bloquesAMoverse);
         
        //uint32_t nuevaPosicionDelArchivo = 
        uint32_t bloqueInicialAux = bloqueInicialArchivoAMover;
        do{
            bitarray_clean_bit(bitmap, bloqueInicialAux);
            bloqueInicialAux++;
        }while(bloqueInicialAux <=  posicionFinalArchivoAMover);

        bloqueInicialAux = bloqueInicialArchivoAMover;
        do{
            log_info(info_logger, "Seteo bit <%d> a <%s>", bloqueInicialAux + bloquesAMoverse, list_get(listaDeArchivos, i));
            bitarray_set_bit(bitmap, bloqueInicialAux + bloquesAMoverse);
            bloqueInicialAux++;

        }while(bloqueInicialAux + bloquesAMoverse <= posicionFinalArchivoAMover + bloquesAMoverse);

        
        void* datosAMover = leerArchivo(list_get(listaDeArchivos, i), bloqueInicialArchivoAMover, tamArchivoAMover);
        //log_info(info_logger, "Estoy pasando cositas: %s", datosAMover);
        escribirArchivo(list_get(listaDeArchivos, i), datosAMover, bloqueInicialArchivoAMover + bloquesAMoverse, tamArchivoAMover);

        char* stringAux= string_itoa(bloqueInicialArchivoAMover + bloquesAMoverse);
        config_set_value(config, "BLOQUE_INICIAL", stringAux);
        config_save(config);
        
        fclose(f);
        free(path); 
        config_destroy(config);

    }


    uint32_t bloquesASumar = (tamanio + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE - 
                        (archivoATruncar->tamArchivo + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE;

    uint32_t tamanioActual = (archivoATruncar->tamArchivo + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE;

    uint32_t posicionFinalActual = (tamanioActual == 0) ? archivoATruncar->bloqueInicial : archivoATruncar->bloqueInicial + tamanioActual - 1;

    if (tengoEspacioAMiLado(archivoATruncar, tamanio)) {  // AGRANDAMOS NORMAL

        log_info(info_logger, "Tengo espacio a mi lado luego de compactar");

        // Si el archivo tenía tamaño 0, debemos empezar la asignación desde el bloque inicial
        uint32_t posicionInicialAsignacion = (tamanioActual == 0) ? archivoATruncar->bloqueInicial : posicionFinalActual + 1;

        //log_debug(debug_logger, "Posicion inicial de asignación <%d> bloques a sumar <%d>", posicionInicialAsignacion, bloquesASumar);

        while (bloquesASumar > 0) {
            bitarray_set_bit(bitmap, posicionInicialAsignacion);
            log_info(info_logger, "Asigno bit <%d> a <%s>", posicionInicialAsignacion, nombreArchivo);
            posicionInicialAsignacion++;
            bloquesASumar--;
        }
}


    logFinCompactacion(pid);


}

//Asumimos que siempre hay espacio disponible y ocupado
uint32_t primerBitDisponible() {

    for(uint32_t i=0; i<bitarray_get_max_bit(bitmap); i++) {
        if(!bitarray_test_bit(bitmap, i))
            return i;
    }
    return -1;
}

uint32_t primerBitOcupado() {

    for(uint32_t i=0; i<bitarray_get_max_bit(bitmap); i++) {
        if(bitarray_test_bit(bitmap, i))
            return i;
    }

    return -1;
}

bool tengoEspacioAMiLado(t_archivo_metadata* archivoATruncar, uint32_t tamanioNuevo) {

    uint32_t tamanioNuevoEnBloques = tamanioNuevo / cfg_entradaSalida->BLOCK_SIZE;  //2

    uint32_t tamanioActual = archivoATruncar->tamArchivo / cfg_entradaSalida->BLOCK_SIZE;   //1

    uint32_t bloquesASumar;

    uint32_t posicionFinalActual;
     
    if(!tamanioActual) {
        posicionFinalActual = archivoATruncar->bloqueInicial;   //no aplica
    }else {
        posicionFinalActual = archivoATruncar->bloqueInicial + tamanioActual - 1;   // 0 + 1 - 1 = 0
    }
    
    if(!tamanioActual) {
        bloquesASumar = tamanioNuevoEnBloques - 1;  //no aplica
    }else {
        bloquesASumar = tamanioNuevoEnBloques - tamanioActual;  //2 - 1;
    }

    uint32_t posicionFinal = posicionFinalActual + bloquesASumar;   //0 + 1 = 1

    for (uint32_t i = posicionFinalActual + 1; i <= posicionFinal; i++) {
        if (bitarray_test_bit(bitmap, i)) {
            log_info(info_logger, "No hay espacio suficiente a mi lado para truncar");
            return false;
        }
    }

    return true;
}

void achicarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar) {
    
    uint32_t bloquesActuales = (archivoATruncar->tamArchivo + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE;
    uint32_t bloquesARestar = (archivoATruncar->tamArchivo - tamanio + cfg_entradaSalida->BLOCK_SIZE - 1) / cfg_entradaSalida->BLOCK_SIZE;

    log_info(info_logger, "Bloques a restar: %d", bloquesARestar);

    for(uint32_t i = 0; i < bloquesARestar; i++) {
        bitarray_clean_bit(bitmap, bloquesActuales - 1 - i);
    }
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

    //log_info(info_logger, "Escribo esta data: %s del tam %d", datos, tamanioAEscribir);

    char* pathArchivoBloques = string_new();
    string_append(&pathArchivoBloques, cfg_entradaSalida->PATH_BASE_DIALFS);
    string_append(&pathArchivoBloques, "/bloques.dat");

    archivoBloques->fd = open(pathArchivoBloques, O_CREAT| O_RDWR,  S_IRUSR|S_IWUSR);

    archivoBloques->direccionArchivo = mmap(NULL,archivoBloques->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, archivoBloques->fd , 0);

    memcpy(archivoBloques->direccionArchivo + direccionAEscribir, datos, tamanioAEscribir);
    
    msync(archivoBloques->direccionArchivo, archivoBloques->tamanio, MS_SYNC);

    close(archivoBloques->fd);
   // free(archivoBloques->fd);
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

    //log_info(info_logger, "La data que lei es: %s", datos);

    close(archivoBloques->fd);
    return datos;
}
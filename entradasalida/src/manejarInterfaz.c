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

    FILE* f = fopen(base, "wb");
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


    list_add(listaArchivos, archivoCreado);

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

    msync(bitmap, cfg_entradaSalida->BLOCK_COUNT, MS_SYNC);
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
/*
void truncarArchivo (char* nombreArchivo, uint32_t tamanio) {
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

    if(archivoATruncar->tamArchivo >= tamanio){
		achicarArchivo(nombreArchivo, tamanio);
        //log_info(info_logger, "PID: <%d> - Tamaño Actual: <%d> - Tamaño a Reducir: <%d>", pid, archivoATruncar->tamArchivo, tamanio);
	}else{
        //log_info(info_logger, "PID: <%d> - Tamaño Actual: <%d> - Tamaño a Ampliar: <%d>", pid, tamOriginal, tamanio);
	}

    fclose(f);

}
*/
void achicarArchivo(char* nombreArchivo, uint32_t tamanio) {

}
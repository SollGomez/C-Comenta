#include "memoriaInstrucciones.h"


void* espacio_contiguo;
//uint32_t *marcosAsignados;
char* charAux[50];

bool memoriaInicializada;
bool tablaPaginasCreada;
bool pagina0Creada;
bool semaforosCreados;

t_list* tablasPaginas;

pthread_mutex_t mutex_espacioContiguo;
pthread_mutex_t mutex_tablasPaginas;
pthread_mutex_t mutex_idPagina;
pthread_mutex_t mutex_espacioDisponible;

t_log* trace_logger;
t_log* debug_logger;
t_log* warning_logger;
t_log* error_logger;
Pagina* pagina0;
t_list* tablasPaginas;
Pagina *paginaParaAnalizar;

int TAM_MEMORIA;
int TAM_PAGINA;
int CANT_MARCOS;

/*bool inicializarBitmap() {
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    CANT_MARCOS = TAM_MEMORIA / TAM_PAGINA;

    marcosAsignados = (uint32_t*)malloc(CANT_MARCOS * sizeof(uint32_t));

    if (marcosAsignados == NULL)
        return false;

    for (int i = 0; i < CANT_MARCOS; i++) {
        marcosAsignados[i] = 0;
    }
    return true;
}*/

void liberarMemory() {
    free(marcosAsignados);
    marcosAsignados = NULL;
}

void liberarPagina(Pagina* pagina) {
    free(pagina); //AVISAR A FS
}

bool crearSemaforos(){
    int comprobacionEspacioContiguo = pthread_mutex_init(&mutex_espacioContiguo,NULL);
    int comprobacionEspacioDisponible = pthread_mutex_init(&mutex_espacioDisponible,NULL);
    int comprobacionIdPagina = pthread_mutex_init(&mutex_idPagina,NULL);
    int comprobacionTablasPaginas = pthread_mutex_init(&mutex_tablasPaginas,NULL);
    int comprobacionFSMutex = pthread_mutex_init(&mutexFS, NULL);


    bool noEsIgualACero(int numero){
        return numero != 0;
    }
    if(noEsIgualACero(comprobacionFSMutex)){
    	log_error(error_logger, "No se pudieron inicializar los semaforos");
    	return false;
    }
    if(noEsIgualACero(comprobacionEspacioContiguo) || noEsIgualACero(comprobacionEspacioDisponible)){
        log_error(error_logger, "No se pudieron inicializar los semaforos");
        return false;
    }
    if(noEsIgualACero(comprobacionIdPagina) || noEsIgualACero(comprobacionTablasPaginas)){
        log_error(error_logger, "No se pudieron inicializar los semaforos");
        return false;
    }
    semaforosCreados = true;
    return true;
}

bool crearEspacioContiguoDeMemoria(){

    espacio_contiguo = malloc(TAM_MEMORIA);
    memset(espacio_contiguo,0,TAM_MEMORIA);
    memoriaInicializada = true;
    return true;
}

/*bool crearEstructurasAdministrativas(){
    bool comp1 = crearSemaforos();
    bool comp2 = crearEspacioContiguoDeMemoria();
    bool comp3 = inicializarBitmap();
    return comp1 && comp2 && comp3;
}

bool iniciarMemoria(){
    bool estructurasAdministrativas = crearEstructurasAdministrativas();//Crea y comprueba que se hayan inicializado todas las estructuras administrativas
    return estructurasAdministrativas;//Retorna si se pudieron crear las estructuras administrativas
}*/

char* BuscarNombreArchivo(uint32_t pid){
    bool coincideProceso(void* archivo){    //hay un "casteo" de tipos para que coincida con la funcion list_find
        NombreArchivo *arch = (NombreArchivo*)archivo;
        return arch->pid == pid;
    }
    NombreArchivo* archivo =  list_find(archivosPseudocodigo, coincideProceso);
    return archivo->nombre;
}

Instruccion* retornarInstruccionACPU(uint32_t pid, uint32_t pc){
	Instruccion* instruccion = malloc(sizeof(Instruccion));
    char* path = string_new();
    char linea[60];

    char* file_name = BuscarNombreArchivo(pid);
    string_append(&path, "/home/utnso/scripts-pruebas"); //CAMBIAR A: PATH_INSTRUCCIONES
	string_append(&path, "/");
	string_append(&path, file_name);

	FILE* archivoPseudocodigo;
	archivoPseudocodigo = fopen(path, "r");
	if(archivoPseudocodigo == NULL) printf("El archivo no existe");    //CAMBIAR A: log_info(info_logger, "No se pudo abrir el archivo");

    for(int i=0; i<pc; i++){ //PREGUNTAR: si pc es 2 va a leer la linea 2
        strcpy(linea, "");
        fgets(linea, sizeof(linea), archivoPseudocodigo);
    }
	fclose(archivoPseudocodigo);

    printf("Lei: %s\n", linea);
    
    char** partesInstruccion = string_split(linea, " ");
    instruccion->idLength = string_length(partesInstruccion[0]);
    instruccion->id = partesInstruccion[0];
    int cantParametros = string_array_size(partesInstruccion)-1;
    instruccion->cantidadParametros = cantParametros;
    
    while (cantParametros != 0){
        switch (cantParametros){
        case 5: 
            instruccion->param5Length = strlen(partesInstruccion[5]);
            instruccion->param5 = partesInstruccion[5];
            instruccion->param4Length = strlen(partesInstruccion[4]);
            instruccion->param4 = partesInstruccion[4];
            break;
        case 3:
            instruccion->param3Length = strlen(partesInstruccion[3]);
            instruccion->param3 = partesInstruccion[3];
            break;
        case 2:
            instruccion->param2Length = strlen(partesInstruccion[2]);
            instruccion->param2 = partesInstruccion[2];
            break;
        case 1:
            instruccion->param1Length = strlen(partesInstruccion[1]);
            instruccion->param1 = partesInstruccion[1];
            break;
        }
        cantParametros--;
    }
	return instruccion;
} 
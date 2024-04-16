#include "memoriaInstrucciones.h"


void* espacio_contiguo;
uint32_t *marcosAsignados;
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

bool inicializarBitmap() {
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
}

void liberarMemory() {
    free(marcosAsignados);
    marcosAsignados = NULL;
}

void liberarPagina(Pagina* pagina) {
    free(pagina); //AVISAR A FS
}

void liberarTablaDePaginas(uint32_t pid) {

	bool compararPorPID(void* elemento, void* pid) {
	    TablaDePaginas* tabla = (TablaDePaginas*)elemento;
	    uint32_t* pidBuscado = (uint32_t*)pid;
	    return tabla->pid == *pidBuscado;
	}
    int indice = -1;
    for (int i = 0; i < list_size(tablaGeneral); i++) {
        TablaDePaginas* tabla = list_get(tablaGeneral, i);
        if (compararPorPID(tabla, &pid)) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        TablaDePaginas* tabla = list_remove(tablaGeneral, indice);
        free(tabla->paginas);
        free(tabla);
        log_info(info_logger, "Tabla con PID <%d> eliminada de tablaGeneral", pid);
    } else {
        log_info(info_logger, "No se encontró una tabla con PID <%d> en tablaGeneral", pid);
    }

}

bool crearEstructurasAdministrativas(){
    bool comp1 = crearSemaforos();
    bool comp2 = crearEspacioContiguoDeMemoria();
    bool comp3 = inicializarBitmap();
    return comp1 && comp2 && comp3;
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

bool iniciarMemoria(){
    bool estructurasAdministrativas = crearEstructurasAdministrativas();//Crea y comprueba que se hayan inicializado todas las estructuras administrativas
    return estructurasAdministrativas;//Retorna si se pudieron crear las estructuras administrativas
}

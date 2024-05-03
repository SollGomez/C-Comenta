#include "esquema.h"

bool flagComunicacion = false;
uint32_t *marcosAsignados;
int RETARDO_RESPUESTA;
pthread_mutex_t mutexFS;

espacioContiguoMemoria espacioUsuario;

uint32_t buscarMarcoLibre() {
    for (uint32_t i = 0; i < CANT_MARCOS; ++i) {
        if (marcosAsignados[i] == 0)
            return i;
    }
    return -1;//indica hay un Page Fault
}

void marcarMarcoOcupado(int numeroMarco) {
    if (numeroMarco >= 0 && numeroMarco < CANT_MARCOS) {
        marcosAsignados[numeroMarco] = 1;
    } else {
        printf("Número de marco inválido\n");
    }
}

void marcarMarcoLibre(int numeroMarco) {
    if (numeroMarco >= 0 && numeroMarco < CANT_MARCOS) {
        marcosAsignados[numeroMarco] = 0;
    } else {
        printf("Número de marco inválido\n");
    }
}

void crearTablaPaginasProceso(uint32_t pid, uint32_t size){
	TablaDePaginas* nuevaTabla = malloc(sizeof (TablaDePaginas));
	  if (nuevaTabla == NULL) {
	        log_info(info_logger, "Error: No se pudo asignar memoria para nuevaTabla\n");
	        exit(EXIT_FAILURE);
	    }

	  int cantPaginas = size / TAM_PAGINA;
	  nuevaTabla->pid = pid;
	    nuevaTabla->paginas = list_create();
	for(int i = 0; i < cantPaginas ; i++){
		Pagina* nuevaPagina = malloc(sizeof(Pagina));
	    if (nuevaPagina == NULL) {
	        printf("Error: No se pudo asignar memoria para nuevaPagina\n");
	        exit(EXIT_FAILURE);
	    }
		nuevaPagina->marco = buscarMarcoLibre();
        printf("MARCO ELEGIDO PARA LA PAGINA %d: %d\n", i, nuevaPagina->marco);
        list_add(nuevaTabla->paginas, nuevaPagina);
		marcarMarcoOcupado(nuevaPagina->marco);
	}
	pthread_mutex_lock(&mutex_tablasPaginas);
    list_add(tablaGeneral, nuevaTabla);
    log_info(info_logger,"Tamaño de tablaGeneral después de iniciar proceso: %d\n", list_size(tablaGeneral));
    pthread_mutex_unlock(&mutex_tablasPaginas);
    log_info(info_logger, "PID: <%d> - Tamaño: <%d>", pid, cantPaginas);
}

void finalizacionDeProceso(uint32_t pid){
	TablaDePaginas* tabla = obtenerTablaPorPID(pid);
    printf("TAMAÑO TABLA: %d\n", list_size(tabla->paginas));
    for (int j = 0; j < list_size(tabla->paginas); j++) {
        Pagina* pagina = list_get(tabla->paginas, j);
        printf("MARCO DE LA PAGINA %d: %d\n",j, pagina->marco);
        marcarMarcoLibre(pagina->marco);
    }
	liberarTablaDePaginas(pid);
	log_info(info_logger,"Tamaño de tablaGeneral después de finalizar proceso: %d\n", list_size(tablaGeneral));
	return;
};

TablaDePaginas* obtenerTablaPorPID(uint32_t pid) {
    for (int i = 0; i < list_size(tablaGeneral); i++) {
        TablaDePaginas* tabla = list_get(tablaGeneral, i);
        if (tabla->pid == pid)
            return tabla;
    }
    return NULL;
}

void liberarTablaDePaginas(uint32_t pid){
    TablaDePaginas* tabla = obtenerTablaPorPID(pid);
    TablaDePaginas* tabla2;
    for (int i=0 ; i<list_size(tablaGeneral); i++){
        tabla2 = list_get(tablaGeneral,i);
        if (tabla->pid == tabla2->pid){
            pthread_mutex_lock(&mutex_tablasPaginas);
            void* element = list_remove(tablaGeneral, i);
            pthread_mutex_unlock(&mutex_tablasPaginas);
            free(tabla->paginas);
            free(tabla);
            log_info(info_logger, "Tabla con PID <%d> eliminada de tablaGeneral", pid);
            return;
        }
    }
    log_info(info_logger, "No se encontró una tabla con PID <%d> en tablaGeneral", pid);
}

uint32_t obtenerMarcoDePagina(uint32_t pid, uint32_t numeroPagina){
	TablaDePaginas* tabla = obtenerTablaPorPID(pid);
	Pagina* pagina = list_get(tabla->paginas, numeroPagina);
	if (pagina->marco >= 0 && pagina->marco < CANT_MARCOS){
		log_info(info_logger,"PID: <%d> - Pagina: <%d> - Marco: <%d>", pid, numeroPagina,pagina->marco);
		return pagina->marco;
	}
	else
		return -1;//HAY QUE VER QUE PASA SI HAY OUT OF MEMORY
}
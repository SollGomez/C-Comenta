#include "PCB.h"

PCB* crearPcb(char* path){
	PCB *pcb = malloc(sizeof(PCB));
	pcb->id = idProcesoGlobal;
	pcb->program_counter = 0;
	pcb->registros = crearRegistroCPU();
	pcb->listaInstrucciones = list_create();
	pcb->tablaPaginas = list_create();
	pcb->recursosTomados = list_create();
	pcb->tiempoEjecutando = 0;
	pcb->nombreRecurso = malloc(strlen(path)+1);
    pcb->quantum = 0;
	strcpy(pcb->nombreRecurso, path);

	idProcesoGlobal++;

  return pcb;
}

RegistrosCPU* crearRegistroCPU() {
	RegistrosCPU* puntero_registros = malloc(sizeof(RegistrosCPU));

    strcpy(puntero_registros->registro_AX, "0");
    strcpy(puntero_registros->registro_BX, "0");
    strcpy(puntero_registros->registro_CX, "0");
    strcpy(puntero_registros->registro_DX, "0");
    strcpy(puntero_registros->registro_EAX, "0000");
    strcpy(puntero_registros->registro_EBX, "0000");
    strcpy(puntero_registros->registro_ECX, "0000");
    strcpy(puntero_registros->registro_EDX, "0000");
    strcpy(puntero_registros->registro_SI, "0000");
    strcpy(puntero_registros->registro_DI, "0000");

    return puntero_registros;
}
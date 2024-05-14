#include "utilidades.h"

Instruccion* recibirInstruccion(int conexion){
	uint32_t tamanioBuffer;
	uint32_t desplazamiento = 0;
	void *buffer = recibir_buffer(&tamanioBuffer, conexion);
	Instruccion* instruccion = malloc(sizeof(Instruccion));

	memcpy(&(instruccion->cantidadParametros), buffer + desplazamiento, sizeof(uint8_t));
	desplazamiento += sizeof(uint8_t);

	memcpy(&(instruccion->idLength), buffer + desplazamiento, sizeof(uint8_t));
	desplazamiento += sizeof(uint8_t);

	instruccion->id = malloc(instruccion->idLength + 1);
	memcpy(instruccion->id, buffer + desplazamiento, instruccion->idLength + 1);
	desplazamiento += instruccion->idLength + 1;

	if (instruccion->cantidadParametros == 1) {
		memcpy(&(instruccion->param1Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param1 = malloc(instruccion->param1Length + 1);
		memcpy(instruccion->param1, buffer + desplazamiento, instruccion->param1Length + 1);
		desplazamiento += instruccion->param1Length + 1;
	}

	if (instruccion->cantidadParametros == 2) {
		memcpy(&(instruccion->param1Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param1 = malloc(instruccion->param1Length + 1);
		memcpy(instruccion->param1, buffer + desplazamiento, instruccion->param1Length + 1);
		desplazamiento += instruccion->param1Length + 1;

		memcpy(&(instruccion->param2Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param2 = malloc(instruccion->param2Length + 1);
		memcpy(instruccion->param2, buffer + desplazamiento, instruccion->param2Length + 1);
		desplazamiento += instruccion->param2Length + 1;
	}

    if (instruccion->cantidadParametros == 3) {
		memcpy(&(instruccion->param1Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param1 = malloc(instruccion->param1Length + 1);
		memcpy(instruccion->param1, buffer + desplazamiento, instruccion->param1Length + 1);
		desplazamiento += instruccion->param1Length + 1;

		memcpy(&(instruccion->param2Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param2 = malloc(instruccion->param2Length + 1);
		memcpy(instruccion->param2, buffer + desplazamiento, instruccion->param2Length + 1);
		desplazamiento += instruccion->param2Length + 1;

        memcpy(&(instruccion->param3Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param3 = malloc(instruccion->param3Length + 1);
		memcpy(instruccion->param3, buffer + desplazamiento, instruccion->param3Length + 1);
		desplazamiento += instruccion->param3Length + 1;
	}

    if (instruccion->cantidadParametros == 4) {
		memcpy(&(instruccion->param1Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param1 = malloc(instruccion->param1Length + 1);
		memcpy(instruccion->param1, buffer + desplazamiento, instruccion->param1Length + 1);
		desplazamiento += instruccion->param1Length + 1;

		memcpy(&(instruccion->param2Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param2 = malloc(instruccion->param2Length + 1);
		memcpy(instruccion->param2, buffer + desplazamiento, instruccion->param2Length + 1);
		desplazamiento += instruccion->param2Length + 1;

        memcpy(&(instruccion->param3Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param3 = malloc(instruccion->param3Length + 1);
		memcpy(instruccion->param3, buffer + desplazamiento, instruccion->param3Length + 1);
		desplazamiento += instruccion->param3Length + 1;

        memcpy(&(instruccion->param4Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param4 = malloc(instruccion->param4Length + 1);
		memcpy(instruccion->param4, buffer + desplazamiento, instruccion->param4Length + 1);
		desplazamiento += instruccion->param4Length + 1;
	}

	if (instruccion->cantidadParametros == 5) {
		memcpy(&(instruccion->param1Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param1 = malloc(instruccion->param1Length + 1);
		memcpy(instruccion->param1, buffer + desplazamiento, instruccion->param1Length + 1);
		desplazamiento += instruccion->param1Length + 1;

		memcpy(&(instruccion->param2Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param2 = malloc(instruccion->param2Length + 1);
		memcpy(instruccion->param2, buffer + desplazamiento, instruccion->param2Length + 1);
		desplazamiento += instruccion->param2Length + 1;

        memcpy(&(instruccion->param3Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param3 = malloc(instruccion->param3Length + 1);
		memcpy(instruccion->param3, buffer + desplazamiento, instruccion->param3Length + 1);
		desplazamiento += instruccion->param3Length + 1;

        memcpy(&(instruccion->param4Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param4 = malloc(instruccion->param4Length + 1);
		memcpy(instruccion->param4, buffer + desplazamiento, instruccion->param4Length + 1);
		desplazamiento += instruccion->param4Length + 1;

		memcpy(&(instruccion->param5Length), buffer + desplazamiento, sizeof(uint8_t));
		desplazamiento += sizeof(uint8_t);

		instruccion->param5 = malloc(instruccion->param5Length + 1);
		memcpy(instruccion->param5, buffer + desplazamiento, instruccion->param5Length + 1);
		desplazamiento += instruccion->param5Length + 1;
	}

	return instruccion;
}

void liberarInstruccion(Instruccion * instruccion){
    if(instruccion->cantidadParametros==1)
        free(instruccion->param1);

    if(instruccion->cantidadParametros==2){
        free(instruccion->param1);
        free(instruccion->param2);
    }
	if(instruccion->cantidadParametros==3){
        free(instruccion->param1);
        free(instruccion->param2);
		free(instruccion->param3);
    }
	if(instruccion->cantidadParametros==4){
        free(instruccion->param1);
        free(instruccion->param2);
		free(instruccion->param3);
		free(instruccion->param4);
    }
	if(instruccion->cantidadParametros==5){
        free(instruccion->param1);
        free(instruccion->param2);
		free(instruccion->param3);
		free(instruccion->param4);
		free(instruccion->param5);
    }

    free(instruccion->id);
    free(instruccion);
}

bool agregarUint32_tsAPaquete(t_list* listaInts, t_paquete* paquete)
{

    paquete->buffer->size+= sizeof(uint32_t)*list_size(listaInts);
    paquete->buffer->size += sizeof(uint8_t);

    void* stream = malloc(paquete->buffer->size);
    int offset=0;

    void copiarElementos(uint32_t* unEntero){
        memcpy(stream + offset, unEntero, sizeof(uint32_t));
        offset+= sizeof(uint32_t);
    }
    int cantidad_ints = list_size(listaInts);
    memcpy(stream + offset, &cantidad_ints, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    list_iterate(listaInts,copiarElementos);
    paquete->buffer->stream = stream;
    return true;
}

bool enviarListaUint32_t(t_list* listaInts, int socket_cliente, t_log* logger, op_code_cliente codigo)
{
    t_paquete* paquete = crear_paquete(codigo, logger);
    if(!agregarUint32_tsAPaquete(listaInts, paquete)){
        log_error(logger, "Hubo un error cuando se intento agregar las instrucciones al paquete");
        return false;
    }
    enviar_paquete(paquete, socket_cliente);
    log_info(logger, "Se envio el paquete");
    eliminar_paquete(paquete);
    return true;
}

void agregar_registros_a_paquete(t_paquete* paquete, RegistrosCPU* registro) {

    //Los valores a almacenar en los registros siempre tendrán la misma longitud que el registro,
    // es decir que si el registro es de 16 bytes siempre se escribirán 16 caracteres.

    uint32_t tamanioAx = 2;

    agregar_a_paquete2(paquete, &(tamanioAx), sizeof(uint32_t));

	agregar_a_paquete2(paquete, (registro->registro_AX), tamanioAx);
	agregar_a_paquete2(paquete, (registro->registro_BX), tamanioAx);
	agregar_a_paquete2(paquete, (registro->registro_CX), tamanioAx);
	agregar_a_paquete2(paquete, (registro->registro_DX), tamanioAx);

	uint32_t tamanioEax = 5;

    agregar_a_paquete2(paquete, &(tamanioEax), sizeof(uint32_t));

	agregar_a_paquete2(paquete, (registro->registro_EAX), tamanioEax);
	agregar_a_paquete2(paquete, (registro->registro_EBX), tamanioEax);
	agregar_a_paquete2(paquete, (registro->registro_ECX), tamanioEax);
	agregar_a_paquete2(paquete, (registro->registro_EDX), tamanioEax);
	agregar_a_paquete2(paquete, (registro->registro_SI), tamanioEax);
	agregar_a_paquete2(paquete, (registro->registro_DI), tamanioEax);

}

void agregar_ContextoEjecucion_a_paquete(t_paquete *paquete, PCB* pcb) {
    //PCB(UINTS_32T): ID
    agregar_a_paquete2(paquete, &(pcb->id), sizeof(uint32_t));
    agregar_a_paquete2(paquete, &(pcb->program_counter), sizeof(uint32_t));
    //la parte de la PCB que no son uint32_t
    //PCB: REGISTROS CPU
    agregar_registros_a_paquete(paquete, pcb->registros);
}

PCB* recibir_contextoEjecucion(int conexion) {
    //Pido el stream del buffer en el que viene serilizada la pcb
    uint32_t tamanioBuffer;
    uint32_t desplazamiento = 0;
    void *buffer = recibir_buffer(&tamanioBuffer, conexion);

    //Inicializo todas las estructuras que necesito
    PCB *PcbRecv = malloc(sizeof(PCB));
    RegistrosCPU *registros = malloc(sizeof(RegistrosCPU));


    //ID
    memcpy(&(PcbRecv->id), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    //Program counter
    memcpy(&(PcbRecv->program_counter), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    //la parte de la PCB que no son uint32_t
    //PCB: REGISTROS CPU

    //REGISTROS CPU 

    uint32_t tamanioAx = 0;

    memcpy(&tamanioAx, buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    //Una vez obtenidos los tamaños, obtenemos los registros
    memcpy(&(registros->registro_AX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_BX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_CX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_DX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;

	uint32_t tamanioEax = 0;

	memcpy(&tamanioEax, buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

	//Una vez obtenidos los tamaños, obtenemos los registros
    memcpy(&(registros->registro_EAX), buffer + desplazamiento, tamanioEax);
    desplazamiento += tamanioEax;
    memcpy(&(registros->registro_EBX), buffer + desplazamiento, tamanioEax);
    desplazamiento += tamanioEax;
    memcpy(&(registros->registro_ECX), buffer + desplazamiento, tamanioEax);
    desplazamiento += tamanioEax;
    memcpy(&(registros->registro_EDX), buffer + desplazamiento, tamanioEax);
    desplazamiento += tamanioEax;
	memcpy(&(registros->registro_SI), buffer + desplazamiento, tamanioEax);
    desplazamiento += tamanioEax;
    memcpy(&(registros->registro_DI), buffer + desplazamiento, tamanioEax);
    desplazamiento += tamanioEax;

    PcbRecv->registros = registros;

    free(buffer);

    return PcbRecv;
}

void liberarPcbCpu(PCB* pcb){
    free(pcb->registros);
    free(pcb);
}

uint32_t recibirValor_uint32(int socket) {

    t_paquete *paquete = malloc(sizeof (t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = 0;
    paquete->buffer->stream =recibir_stream(&paquete->buffer->size, socket);
    uint32_t valor = -1;
    memcpy(&valor, paquete->buffer->stream, sizeof(uint32_t));
    eliminar_paquete(paquete);

    return valor;
}

void enviar_uint32_y_uint32_y_char(char* path, uint32_t valor1, uint32_t valor2, int socket, op_code_cliente orden, t_log *logger){
    t_paquete * paquete= crear_paquete(orden, logger);
    agregar_a_paquete2(paquete, &valor1, sizeof(uint32_t));
    agregar_a_paquete2(paquete, &valor2, sizeof(uint32_t));

    uint32_t largo_nombre = strlen(path) + 1;
	agregar_a_paquete2(paquete, &largo_nombre, sizeof(uint32_t));
	agregar_a_paquete2(paquete, path, largo_nombre);

    enviar_paquete(paquete,socket);
    eliminar_paquete(paquete);
}

void recibirOrden(int socket){
    int tamanio;
    void* stream = recibir_stream(&tamanio, socket);
    free (stream);
}

void enviarOrden(op_code_cliente orden, int socket, t_log *logger) {
    t_paquete * paquete= crear_paquete(orden, logger);
    paquete->buffer->size+=sizeof (uint32_t);
    void* stream = malloc(paquete->buffer->size);
    uint32_t valor = 0;
    int offset= 0;
    memcpy(stream + offset, &valor, sizeof(uint32_t));
    paquete->buffer->stream = stream;

    enviar_paquete(paquete,socket);
    eliminar_paquete(paquete);
    free(valor);
    free(offset);
}

void enviarValor_uint32(uint32_t valor, int socket, op_code_cliente orden, t_log *logger)
{
    t_paquete * paquete= crear_paquete(orden, logger);
    paquete->buffer->size = sizeof(uint32_t);
    void* stream = malloc(paquete->buffer->size);
    int offset = 0;
    memcpy(stream + offset, &valor, sizeof(uint32_t));
    paquete->buffer->stream = stream;
    enviar_paquete(paquete,socket);
    log_info(logger, "se envio el paquete");
    eliminar_paquete(paquete);
}

void* recibir_stream(int* size, uint32_t cliente_socket) {
    recv(cliente_socket, size, sizeof(int), MSG_WAITALL);
    void* stream = malloc(*size);
    recv(cliente_socket, stream, *size, MSG_WAITALL);
    return stream;
}

PCB* recibir_contextoEjecucion_y_char (int conexion) {
    uint32_t tamanioBuffer;
    uint32_t desplazamiento = 0;
    void *buffer = recibir_buffer(&tamanioBuffer, conexion);

    PCB *PcbRecv = malloc(sizeof(PCB));
    RegistrosCPU *registros = malloc(sizeof(RegistrosCPU));

    memcpy(&(PcbRecv->id), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    memcpy(&(PcbRecv->program_counter), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    uint32_t tamanioAx = 0;

    memcpy(&tamanioAx, buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    memcpy(&(registros->registro_AX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_BX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_CX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_DX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;

    PcbRecv->registros = registros;

    uint32_t tamanioPalabra=0;
	memcpy(&tamanioPalabra, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	PcbRecv->nombreRecurso = malloc(tamanioPalabra + 1);
	memcpy(PcbRecv->nombreRecurso, buffer + desplazamiento, tamanioPalabra + 1);
	desplazamiento += tamanioPalabra + 1;

    free(buffer);

    return PcbRecv;
}

PCB* recibir_contextoEjecucion_y_uint32(int conexion, uint32_t* direccion) {
    uint32_t tamanioBuffer;
    uint32_t desplazamiento = 0;
    void *buffer = recibir_buffer(&tamanioBuffer, conexion);

    PCB *PcbRecv = malloc(sizeof(PCB));
    RegistrosCPU *registros = malloc(sizeof(RegistrosCPU));

    memcpy(&(PcbRecv->id), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    memcpy(&(PcbRecv->program_counter), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    uint32_t tamanioAx = 0;

    memcpy(&tamanioAx, buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    memcpy(&(registros->registro_AX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_BX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_CX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_DX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;

    PcbRecv->registros = registros;

	memcpy(direccion, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

    free(buffer);

    return PcbRecv;
}

PCB* recibir_contextoEjecucion_y_uint32_y_uint32(int conexion, uint32_t* direccion, uint32_t* direccion2) {
    uint32_t tamanioBuffer;
    uint32_t desplazamiento = 0;
    void *buffer = recibir_buffer(&tamanioBuffer, conexion);

    PCB *PcbRecv = malloc(sizeof(PCB));
    RegistrosCPU *registros = malloc(sizeof(RegistrosCPU));

    memcpy(&(PcbRecv->id), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    memcpy(&(PcbRecv->program_counter), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    uint32_t tamanioAx = 0;

    memcpy(&tamanioAx, buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    memcpy(&(registros->registro_AX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_BX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_CX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;
    memcpy(&(registros->registro_DX), buffer + desplazamiento, tamanioAx);
    desplazamiento += tamanioAx;

    PcbRecv->registros = registros;

	memcpy(direccion, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

    memcpy(direccion2, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

    free(buffer);

    return PcbRecv;
}
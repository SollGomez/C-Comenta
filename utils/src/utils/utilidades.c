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


	return instruccion;
}

void* recibir_stream(int* size, uint32_t cliente_socket) { //En realidad devuelve el stream, no el t_buffer
    recv(cliente_socket, size, sizeof(int), MSG_WAITALL);
    void *buffer = malloc(*size);
    recv(cliente_socket, buffer, *size, MSG_WAITALL);
    return buffer;
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

t_list* recibirListaUint32_t(int socket_cliente){
    int tamanio;
    int desplazamiento = 0;
    void *buffer = recibir_stream(&tamanio, socket_cliente);
    t_list* listaInts = list_create();
    int cantidad_ints = 0;
    memcpy(&cantidad_ints, buffer + desplazamiento, sizeof(uint8_t));
    desplazamiento+=sizeof(uint8_t);

    for (int i = 0; i < cantidad_ints; ++i) {
        uint32_t* nuevoEntero = malloc(sizeof(uint32_t));
        memcpy(nuevoEntero, buffer + desplazamiento, sizeof (uint32_t));
        desplazamiento+=sizeof(uint32_t);
        list_add(listaInts, nuevoEntero);
        //free(nuevoEntero); // Linea que me rompio todo loco, ubiquense. Probablemente un free de kiki seguro.  (bauti)
    }
    free(buffer);
    return listaInts;
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


bool enviarEnteroYString(uint32_t entero,char* string, int socket_cliente, t_log* logger, op_code_cliente codigo)

{
    t_paquete* paquete = crear_paquete(codigo, logger);
    if(!agregarEnteroYStringAPaquete(entero,string, paquete)){
        log_error(logger, "Hubo un error cuando se intento agregar las instrucciones al paquete");
        return false;
    }
    enviar_paquete(paquete, socket_cliente);
    log_info(logger, "Se envio el paquete");
    eliminar_paquete(paquete);
    return true;
}

bool agregarEnteroYStringAPaquete(uint32_t entero, char* string, t_paquete* paquete)
{

    paquete->buffer->size+= sizeof(uint32_t);
    uint32_t tamanioString = strlen(string) +1;
    paquete->buffer->size+= tamanioString + sizeof(uint32_t);


    void* stream = malloc(paquete->buffer->size); //Reservo memoria para el stream
    int offset=0; //desplazamiento



    memcpy(stream + offset, &entero, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(stream + offset, &tamanioString, sizeof(uint32_t));
    offset+= sizeof(uint32_t);
    memcpy(stream + offset, string, tamanioString);
    paquete->buffer->stream = stream;

    return true;

}



char* recibirEnteroYString(int socket_cliente,uint32_t* entero)
{
    int tamanio;
    int desplazamiento = 0;
    void *buffer = recibir_stream(&tamanio, socket_cliente);

    memcpy(&entero, buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento+=sizeof(uint32_t);

    uint32_t tamanioString =0;
    memcpy(&tamanioString, buffer + desplazamiento, sizeof (uint32_t));
    desplazamiento+=sizeof(uint32_t);
    char* string = malloc(tamanioString);
    memcpy(string, buffer + desplazamiento, tamanioString);

    free(buffer);
    return string;

}
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
#include "utilidades.h"

//REVISAR TODAS

void* recibir_stream(int* size, uint32_t cliente_socket) { //En realidad devuelve el stream, no el t_buffer
    recv(cliente_socket, size, sizeof(int), MSG_WAITALL);
    void *buffer = malloc(*size);
    recv(cliente_socket, buffer, *size, MSG_WAITALL);
    return buffer;
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
        free(nuevoEntero); //linea agregada
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

void simularRetardoSinMensaje(int retardo){
    usleep(retardo*1000);
}

//VER porque esta fea... es nueva
void agregar_instruccion_a_paquete(t_paquete *paquete, Instruccion* instruccion) {

        agregar_a_paquete2(paquete, &(instruccion->cantidadParametros), sizeof(uint8_t));
        agregar_a_paquete2(paquete, &(instruccion->idLength), sizeof(uint8_t));
        agregar_a_paquete2(paquete, instruccion->id, instruccion->idLength + 1);


        if(instruccion->cantidadParametros == 1){

            agregar_a_paquete2(paquete, &(instruccion->param1Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param1, instruccion->param1Length+1);          }

        if(instruccion->cantidadParametros == 2){

            agregar_a_paquete2(paquete, &(instruccion->param1Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param1, instruccion->param1Length+1);
            agregar_a_paquete2(paquete,  &(instruccion->param2Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param2, instruccion->param2Length+1);        }
        
        if(instruccion->cantidadParametros == 3){

            agregar_a_paquete2(paquete, &(instruccion->param1Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param1, instruccion->param1Length+1);
            agregar_a_paquete2(paquete,  &(instruccion->param2Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param2, instruccion->param2Length+1); 
            agregar_a_paquete2(paquete,  &(instruccion->param3Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param3, instruccion->param3Length+1);        }
        
        if(instruccion->cantidadParametros == 5){

            agregar_a_paquete2(paquete, &(instruccion->param1Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param1, instruccion->param1Length+1);
            agregar_a_paquete2(paquete,  &(instruccion->param2Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param2, instruccion->param2Length+1); 
            agregar_a_paquete2(paquete,  &(instruccion->param3Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param3, instruccion->param3Length+1);
            agregar_a_paquete2(paquete,  &(instruccion->param4Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param4, instruccion->param4Length+1);
            agregar_a_paquete2(paquete,  &(instruccion->param5Length), sizeof(uint8_t));
            agregar_a_paquete2(paquete, instruccion->param5, instruccion->param5Length+1);         }

}

bool agregarIntsYDatosAPaquete(t_list* listaInts, t_datos* datos, t_paquete* paquete){

    paquete->buffer->size+= sizeof(uint32_t)*list_size(listaInts);
    paquete->buffer->size+= datos->tamanio + sizeof(uint32_t);

    paquete->buffer->size += sizeof(uint8_t);
    void* stream = malloc(paquete->buffer->size);
    int offset=0;

    void copiarElementos(uint32_t* unEntero){
        memcpy(stream + offset, unEntero, sizeof(uint32_t));
        offset+= sizeof(uint32_t);
    }
    uint8_t cantidad_ints = list_size(listaInts);
    memcpy(stream + offset, &cantidad_ints, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    list_iterate(listaInts,copiarElementos);
    memcpy(stream + offset, &datos->tamanio, sizeof(uint32_t));
    offset+= sizeof(uint32_t);
    memcpy(stream + offset, datos->datos, datos->tamanio);
    paquete->buffer->stream = stream;
    return true;

}

bool enviarListaIntsYDatos(t_list* listaInts,t_datos* datos, int socket_cliente, t_log* logger, op_code_cliente codigo)
{
    t_paquete* paquete = crear_paquete(codigo, logger);
    if(!agregarIntsYDatosAPaquete(listaInts, datos, paquete)){
        log_error(logger, "Hubo un error cuando se intento agregar las instrucciones al paquete");
        return false;
    }
    enviar_paquete(paquete, socket_cliente);
    log_info(logger, "Se envio el paquete");
    eliminar_paquete(paquete);
    return true;
}

t_list* recibirListaIntsYDatos(int socket_cliente,t_datos* datos){
    int tamanio;
    int desplazamiento = 0;
    void *buffer = recibir_stream(&tamanio, socket_cliente);
    t_list* listaInts = list_create();
    uint8_t cantidad_ints = 0;
    memcpy(&cantidad_ints, buffer + desplazamiento, sizeof(uint8_t));
    desplazamiento+=sizeof(uint8_t);

    for (int i = 0; i < cantidad_ints; ++i) {
        uint32_t* nuevoEntero = malloc(sizeof(uint32_t));
        memcpy(nuevoEntero, buffer + desplazamiento, sizeof (uint32_t));
        desplazamiento+=sizeof(uint32_t);
        list_add(listaInts, nuevoEntero);
    }

    memcpy(&datos->tamanio, buffer + desplazamiento, sizeof (uint32_t));
    desplazamiento+=sizeof(uint32_t);
    datos->datos = malloc(datos->tamanio);
    memcpy(datos->datos, buffer + desplazamiento, datos->tamanio);

    free(buffer);
    return listaInts;

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

char* recibirEnteroEnteroChar(int socket_cliente, uint32_t* entero1, uint32_t* entero2){
	uint32_t tamanioBuffer;
	uint32_t desplazamiento = 0;
	void *buffer = recibir_buffer(&tamanioBuffer, socket_cliente);

	memcpy(entero1, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(entero2, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	uint32_t tamanioPalabra=0;
	memcpy(&tamanioPalabra, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	char* palabraRecibida = malloc(tamanioPalabra + 1);
	memcpy(palabraRecibida, buffer + desplazamiento, tamanioPalabra + 1);
	desplazamiento += tamanioPalabra + 1;

	return palabraRecibida;
}
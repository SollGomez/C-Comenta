#ifndef UTILSCLIENT_H_
#define UTILSCLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <string.h>

typedef enum
{	MENSAJECLIENTE,
	PAQUETECLIENTE,
	CONTEXTOEJECUCION,
	SOLICITUDINSTRUCCION,
	IO_GEN_SLEEP,
	WAIT,
	IO_STDIN_READ,
	IO_STDOUT_WRITE,
	IO_FS_CREATE,
	IO_FS_DELETE,
	IO_FS_TRUNCATE,
	IO_FS_WRITE,
	IO_FS_READ,
	IO_INTERFAZ_ERRONEA
	
}op_code_cliente;

typedef struct
{	int size;
	void* stream;
} t_buffer;

typedef struct
{	op_code_cliente codigo_operacion;
	t_buffer* buffer;
} t_paquete;

int crear_conexion(t_log* logger, const char* server_name, char* ip, char* puerto);
void enviar_mensaje(char*, int);
t_paquete* crear_paquete(op_code_cliente, t_log*);
void agregar_a_paquete(t_paquete*, void*, int);
void agregar_a_paquete2(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete*, int);
void liberar_conexion(int);
void eliminar_paquete(t_paquete*);

#endif /* UTILSCLIENT_H_ */

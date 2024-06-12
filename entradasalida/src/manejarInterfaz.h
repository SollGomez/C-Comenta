#include <clean.h>
#include <utils/utilidades.h>
#include <comunicacion.h>



void cualInterfaz();
void manejarInterfazGenerica(uint32_t unidadesDeTrabajo);
void manejarInterfazStdin(uint32_t direccionFisica);
void crearArchivo(char* nombreArchivo);
void eliminarArchivo(char* nombreArchivo);
void truncarArchivo (char* nombreArchivo, uint32_t tamanio);
uint32_t obtener_bloque_libre (t_bitarray* bitmap);

void truncarArchivo (char* nombreArchivo, uint32_t tamanio);
void achicarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar);
bool tengoEspacioAMiLado(t_archivo_metadata* archivoATruncar, uint32_t tamanioNuevo);
void agrandarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar);
uint32_t hayEspacioContiguo(uint32_t blocksRequested);
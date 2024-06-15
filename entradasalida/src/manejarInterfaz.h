#include <clean.h>
#include <utils/utilidades.h>
#include <comunicacion.h>



void cualInterfaz();
void manejarInterfazGenerica(uint32_t unidadesDeTrabajo);
void manejarInterfazStdin(uint32_t direccionFisica, uint32_t pid);
void crearArchivo(char* nombreArchivo);
void eliminarArchivo(char* nombreArchivo);
void truncarArchivo (char* nombreArchivo, uint32_t tamanio, uint32_t pid);
uint32_t obtener_bloque_libre (t_bitarray* bitmap);

void achicarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar);
bool tengoEspacioAMiLado(t_archivo_metadata* archivoATruncar, uint32_t tamanioNuevo);
void agrandarArchivo(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar, uint32_t pid);
uint32_t hayEspacioContiguo(uint32_t blocksRequested);
void escribirArchivo(char* nombreArchivo, void* datos, uint32_t direccionAEscribir, uint32_t tamanio);
void* leerArchivo(char* nombreArchivo, uint32_t direccionALeer, uint32_t tamanioALeer);
uint32_t primerBitDisponible();
uint32_t primerBitOcupado();
void compactar(char* nombreArchivo, uint32_t tamanio, t_archivo_metadata* archivoATruncar, uint32_t pid);
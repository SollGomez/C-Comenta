#include <clean.h>
#include <utils/utilidades.h>
#include <comunicacion.h>



void cualInterfaz();
void manejarInterfazGenerica(uint32_t unidadesDeTrabajo);
void manejarInterfazStdin(uint32_t direccionFisica);
void crearArchivo(char* nombreArchivo);
void eliminarArchivo(char* nombreArchivo);
uint32_t buscarBloqueLibre();
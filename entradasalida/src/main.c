#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include <clean.h>

int main(int argc, char* argv[]) {

    init_loggers_config(argv[1]);
    cargar_configuracion();
    cerrarPrograma();

    return 0;
}

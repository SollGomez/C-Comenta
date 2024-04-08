#include <stdlib.h>
#include <stdio.h>
#include <utils/shared.h>
#include <clean.h>

int main(int argc, char* argv[]) {

    char* pathConfig = argv[1];
    
    

    init_loggers_config(pathConfig);
    cargar_configuracion();
    cerrarPrograma();

    return 0;
}

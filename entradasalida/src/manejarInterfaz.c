#include <manejarInterfaz.h>


void cualInterfaz() {

    switch (cfg_entradaSalida->TIPO_INTERFAZ_INT)
    {
    case 0:  //STDOUT
        /* code */
        break;
    case 1:  //STDIN
        /* code */
        break;
    case 2:  //DIALFS
        /* code */
        break;
    case 3:  //GENERICA
        break;
    default:
        printf("Esa interfaz no existe :/");
        break;
    }

    return;
}


void manejarInterfazGenerica(uint32_t unidadesDeTrabajo) {
    
    
    for (int i = 0; i < unidadesDeTrabajo; i++) {

        // sleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO);               //DESPUES VER SI ES USLEEP O SLEEP
        usleep(cfg_entradaSalida->TIEMPO_UNIDAD_TRABAJO * 10);
        printf("UNIDAD: %d\n", i);
    }

}
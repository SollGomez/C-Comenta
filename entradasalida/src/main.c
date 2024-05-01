#include <main.h>

t_config* config;

int main(int argc, char* argv[]) {
    decir_hola("una Interfaz de Entrada/Salida");
  
    config = crearConfig(argv[1]);
    init_loggers_config(argv[1]);
    cargar_configuracion();
	crearListas();
	crearSemaforos();
	iniciarAtencionPeticiones();
    
    
	if(cfg_entradaSalida->TIPO_INTERFAZ_INT == 3) {
		conectarKernel("KERNEL");
	}else {
		conectarKernel("KERNEL");
		conectarMemoria("MEMORIA");
	}




	char nombre[50];
	scanf("%s", nombre);

	cerrarPrograma();
    return 0;
}

t_config *crearConfig(char* configPath){
	t_config* config;

	config = config_create(configPath);

	if(config != NULL)
		return config;

	printf("La configuracion no se pudo crear exitosamente");
	exit(2);

	return config;
}
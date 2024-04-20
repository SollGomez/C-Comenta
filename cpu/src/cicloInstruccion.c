#include "cicloInstruccion.h"

bool cicloInstrucciones=true;

char registroCPU_AX[1];
char registroCPU_BX[1];
char registroCPU_CX[1];
char registroCPU_DX[1];
char registroCPU_EAX[4];
char registroCPU_EBX[4];
char registroCPU_ECX[4];
char registroCPU_EDX[4];
char registroCPU_SI[4];
char registroCPU_DI[4];

void ciclo_de_instruccion(){
    
    cicloInstrucciones=true;

    while(cicloInstrucciones){

        fetch();

        decode();

        execute();

        checkInsterrupt();

    }
}

void fetch(){

}

void decode(){

}

void execute(){

}

void checkInsterrupt(){

}

void copiar_registroPCB_a_los_registrosCPU (RegistrosCPU* registro) {
	memcpy(registroCPU_AX,registro->registro_AX,1);
	memcpy(registroCPU_BX,registro->registro_BX,1);
	memcpy(registroCPU_CX,registro->registro_CX,1);
	memcpy(registroCPU_DX,registro->registro_DX,1);
    memcpy(registroCPU_EAX,registro->registro_EAX,4);
	memcpy(registroCPU_EBX,registro->registro_EBX,4);
	memcpy(registroCPU_ECX,registro->registro_ECX,4);
	memcpy(registroCPU_EDX,registro->registro_EDX,4);
    memcpy(registroCPU_SI,registro->registro_SI,4);
	memcpy(registroCPU_DI,registro->registro_DI,4);
}

void copiar_registrosCPU_a_los_registroPCB(RegistrosCPU* registro) {
    memcpy(registro->registro_AX,registroCPU_AX,1);
    memcpy(registro->registro_BX,registroCPU_BX,1);
    memcpy(registro->registro_CX,registroCPU_CX,1);
    memcpy(registro->registro_DX,registroCPU_DX,1);
    memcpy(registro->registro_EAX,registroCPU_EAX,4);
    memcpy(registro->registro_EBX,registroCPU_EBX,4);
    memcpy(registro->registro_ECX,registroCPU_ECX,4);
    memcpy(registro->registro_EDX,registroCPU_EDX,4);
    memcpy(registro->registro_SI,registroCPU_SI,4);
    memcpy(registro->registro_DI,registroCPU_DI,4);
}
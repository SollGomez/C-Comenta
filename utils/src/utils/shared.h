#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdlib.h>
#include <stdio.h>
#include "utilsServer.h"
#include "utilsClient.h"
#include "estructurasCompartidas.h"

bool esInstruccionSinParametros(Instruccion *);
bool esInstruccionConUnParametro(Instruccion *);
bool esInstruccionConDosParametros(Instruccion *);
bool esInstruccionConTresParametros(Instruccion *);
bool esInstruccionConCincoParametros(Instruccion *);


#endif

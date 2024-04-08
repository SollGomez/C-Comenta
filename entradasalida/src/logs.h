#ifndef LOGS_H_
#define LOGS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <string.h>
#include <utils/shared.h>

int init_loggers();

void logOperacion(uint32_t pid, char* operacionARealizar);


#endif /* LOGS_H_ */
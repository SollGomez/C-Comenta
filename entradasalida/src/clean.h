#ifndef CLEAN_H_
#define CLEAN_H_

#include <logs.h>

extern t_log* trace_logger;
extern t_log* info_logger;
extern t_log* error_logger;
extern bool logsCreados;

void cerrarPrograma();
void destruirLoggers();

#endif //CLEAN_H_
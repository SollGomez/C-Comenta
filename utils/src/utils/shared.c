#include <utils/shared.h>

bool esInstruccionSinParametros(Instruccion * instruccion){
    return (strcmp(instruccion->id, "EXIT") == 0);
}

bool esInstruccionConUnParametro(Instruccion * instruccion){
    bool resize = (strcmp(instruccion->id, "RESIZE") == 0);
    bool copy_string = (strcmp(instruccion->id, "COPY_STRING") == 0);
    bool wait = (strcmp(instruccion->id, "WAIT") == 0);
    bool signal = (strcmp(instruccion->id, "SIGNAL") == 0);

    return  resize || copy_string || wait || signal;
}

bool esInstruccionConDosParametros(Instruccion * instruccion){
    bool set = (strcmp(instruccion->id, "SET") == 0);
    bool sum = (strcmp(instruccion->id, "SUM") == 0);
    bool sub = (strcmp(instruccion->id, "SUB") == 0);
    bool mov_in = (strcmp(instruccion->id, "MOV_IN") == 0);
    bool mov_out = (strcmp(instruccion->id, "MOV_OUT") == 0);
    bool jnz = (strcmp(instruccion->id, "JNZ") == 0);

    bool io_gen_sleep = (strcmp(instruccion->id, "IO_GEN_SLEEP") == 0);
    bool io_fs_create = (strcmp(instruccion->id, "IO_FS_CREATE") == 0);
    bool io_fs_delete = (strcmp(instruccion->id, "F_SEEK") == 0);

    return set || sum || sub || mov_in || mov_out || jnz || io_gen_sleep || io_fs_create || io_fs_delete;
}

bool esInstruccionConTresParametros(Instruccion * instruccion){
    bool io_stdin_read = (strcmp(instruccion->id, "IO_STDIN_READ") == 0);
    bool io_stdout_write = (strcmp(instruccion->id, "IO_STDOUT_WRITE") == 0);
    bool io_fs_truncate = (strcmp(instruccion->id, "IO_FS_TRUNCATE") == 0);

    return io_stdin_read || io_stdout_write || io_fs_truncate;
}

bool esInstruccionConCincoParametros(Instruccion * instruccion){
    bool io_fs_write = (strcmp(instruccion->id, "IO_FS_WRITE") == 0);
    bool io_fs_read = (strcmp(instruccion->id, "IO_FS_READ") == 0);

    return io_fs_write || io_fs_read;
}

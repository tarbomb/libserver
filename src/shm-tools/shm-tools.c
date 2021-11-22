/*
 * Basic wrappers around shared-memory functions.
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#include "shm-tools.h"

void shmtools_error(const char *format, ...) {
    va_list specifiers = {0};

    va_start(specifiers, format);
    vfprintf(stderr, format, specifiers);
    va_end(specifiers);

    exit(SHM_TOOLS_EXIT_CODE);
}

int shmtools_file_exists(const char *path) {
    struct stat stat_buffer = {0};

    if(stat(path, &stat_buffer) == -1) {
        return 0;
    }

    return 1;
}

int shmtools_get_id(const char *file, size_t size) {
    int key = ftok(file, size);
    int id = -1;

    /* Failed to get the IPC key */
    if(key == -1) {
        shmtools_error("shmtools_get_id: failed to get ipc key - '%s'\n", strerror(errno));
    }

    id = shmget(key, size, 0644 | IPC_CREAT);

    /* Failed to get the shared memory block */
    if(id == -1) {
        shmtools_error("shmtools_get_id: failed to get shared memory block - '%s'\n", strerror(errno));
    }

    return id;
}

int shmtools_get_id_create(const char *file, size_t size) {
    if(shmtools_file_exists(file) == 0) {
        fclose(fopen(file, "w+"));
    }

    return shmtools_get_id_create(file, size);
}

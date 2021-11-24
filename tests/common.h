/*
 * Common functions and libraries for testing
*/

#ifndef LIB_SERVER_TESTING_COMMON_H
#define LIB_SERVER_TESTING_COMMON_H

#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>

#define LIB_SERVER_TESTING_ERROR_LENGTH 512

/*
 * Extract an error from the stderr of a function.
 *
 * @param length: the length of the buffer to write to
 * @param buffer: the location to write the error message to
 * @param function: the function to invoke
*/
static void libserver_extract_error(size_t length, char buffer[], void (*callback)()) {
    int communication[2];

    pipe(communication);

    /* Parent process should only read from its input pipe. */
    if(fork() != 0) {
        memset(buffer, 0, length);
        close(communication[1]);
        read(communication[0], buffer, length);
        close(communication[0]);

        return;
    }

    /* Set the write end of the pipe as the stderr of
     * this child process */
    close(communication[0]);
    dup2(communication[1], STDERR_FILENO);
    callback(); 
    close(communication[1]);
}

static int libserver_read_with_timeout(int descriptor, char buffer[], size_t length, int timeout) {
    int waited = 0;
    struct pollfd descriptors[1];

    descriptors->fd = descriptor;
    descriptors->events = POLLRDNORM;

    /* Poll until a timeout is reached */
    while(waited < timeout) {
        int events = poll(descriptors, 1, 1000); 

        if(events <= 0) {
            waited++;
            continue;
        }

        if((descriptors->revents & POLLRDNORM) == 0) {
            waited++;
            continue;
        }

        read(descriptor, buffer, length);

        return 0;
    }

    return 1;
}

/*
 * Returns whether or not the file pointed to by the path
 * PATH exists on the file system.
 *
 * @param: the path to check
 * @return: 0 if the path does not exist, 1 if it does
*/
static int libserver_file_exists(const char *path) {
    struct stat stat_buffer = {0};

    if(stat(path, &stat_buffer) == -1) {
        return 0;
    }

    return 1;
}

#endif

/*
 * Common functions and libraries for testing
*/

#ifndef LIB_SERVER_TESTING_COMMON_H
#define LIB_SERVER_TESTING_COMMON_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

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

#endif

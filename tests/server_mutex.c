/*
 * Tests for the server's mutex
*/

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/shm.h>

#include "common.h"
#include "../src/server.h"

/* Minimum amount of time it should take for the server to initialize
 * the mutex */
#define MUTEX_INITIALIZATION_TIMELIMIT 1

void server(void) {
    struct LibServerServer new_server = libserver_init("./test_mutex");

    libserver_free(new_server);
}

/*
 * Open the mutex, a
*/
void client(void) {
    int memory_key = ftok("./test-mutex", 0);
    int memory_id = shmget(memory_id, sizeof(pthread_mutex_t), 0644);
    pthread_mutex_t *mutex = shmat(memory_id, NULL, 0);

}

int main(void) {
    int pipes[2] = {0};     

    pipe(pipes);

    /* Child process */
    if(fork() == 0) {
        client();
        
        return EXIT_SUCCESS;
    }

    server();

    return EXIT_SUCCESS;
}

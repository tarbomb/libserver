/*
 * The main file for controlling the server. The main loop of the server goes roughly
 * like this:
 *
 * 1) Poll for any clients requesting to connect to the server on the server's input pipe
 * 2) Poll for any requests from the connected clients
 * 3) Distribute any events
 * 4) Repeat
*/

#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>

#include "server.h"
#include "shm-tools/shm-tools.h"
#include "data-structures/client.h"

struct LibServerServer libserver_init(const char *mutex_file) {
    int mutex_id = 0;
    pthread_mutexattr_t attribute = {0};
    struct LibServerServer new_server = {0};

    if(mutex_file == NULL) {
        fprintf(stderr, "%s", "libserver_init: attempt to map shared mutex to NULL pointer\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the client array */
    new_server.clients.contents = malloc(LIB_SERVER_DEFAULT_CLIENT_LENGTH * sizeof(struct LibServerClient));
    new_server.clients.physical_size = LIB_SERVER_DEFAULT_CLIENT_LENGTH;

    /* Setup shared memory and new mutex */
    mutex_id = shmtools_get_id_create(mutex_file, sizeof(pthread_mutex_t));
    new_server.mutex = (pthread_mutex_t*) shmtools_attach(mutex_id);

    pthread_mutexattr_init(&attribute);
    pthread_mutexattr_setpshared(&attribute, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(new_server.mutex, &attribute);

    return new_server;
}

void libserver_free(struct LibServerServer server, const char *mutex_file) {
    pthread_mutex_destroy(server.mutex);
    libserver_client_array_free(&server.clients);

    /* Delete the shared memory and mutex file */
    shmtools_destroy(shmtools_get_id(mutex_file, sizeof(pthread_mutex_t)));
    unlink(mutex_file);

    free(server.clients.contents);
}

/*
 * Operations on a server object.
*/

#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "server.h"
#include "client.h"
#include "../shm-tools/shm-tools.h"

void libserver_server_init_mutex(struct LibServerServer *server, const char *directory) {
    int mutex_id = 0;
    char mutex_file[PATH_MAX] = {0};
    pthread_mutexattr_t attribute = {0};

    strcat(mutex_file, directory);
    strcat(mutex_file, LIB_SERVER_MUTEX_NAME);

    mutex_id = shmtools_get_id_create(mutex_file, sizeof(pthread_mutex_t));
    server->mutex = (pthread_mutex_t*) shmtools_attach(mutex_id);

    pthread_mutexattr_init(&attribute);
    pthread_mutexattr_setpshared(&attribute, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(server->mutex, &attribute);
}

struct LibServerServer libserver_server_init(const char *directory) {
    struct LibServerServer new_server = {0};

    if(directory == NULL) {
        fprintf(stderr, "%s", "libserver_init: attempt to generate server information at NULL directory\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the client array */
    new_server.clients.contents = malloc(LIB_SERVER_DEFAULT_CLIENT_LENGTH * sizeof(struct LibServerClient));
    new_server.clients.physical_size = LIB_SERVER_DEFAULT_CLIENT_LENGTH;
    new_server.directory = directory;

    /* Setup shared memory and new mutex */
    libserver_server_init_mutex(&new_server, directory);

    return new_server;
}

void libserver_server_free(struct LibServerServer server, const char *mutex_file) {
    pthread_mutex_destroy(server.mutex);
    libserver_client_array_free(&server.clients);

    /* Delete the shared memory and mutex file */
    shmtools_destroy(shmtools_get_id(mutex_file, sizeof(pthread_mutex_t)));
    unlink(mutex_file);

    free(server.clients.contents);
}

void libserver_server_add_client(struct LibServerServer *server, int process_id) {
    struct LibServerClient new_client = {0};
    char from_client_path[PATH_MAX] = {0};

    /* Open the input pipe for the client, which should have been created
     * by the client before invoking the server. */
    sprintf(from_client_path, "%s/%s/%i.from", server->directory, LIB_SERVER_CLIENT_DIRECTORY, process_id);
    new_client.process_id = process_id;
    new_client.from_client = mkfifo(from_client_path, 0744 | O_CREAT | O_NONBLOCK);

    if(new_client.from_client == -1) {
        fprintf(stderr, "libserver_server_add_client: could not open from_client pipe '%s' - '%s'\n", from_client_path, strerror(errno));
        exit(LIB_SERVER_EXIT_CODE);
    }

    libserver_client_array_append(&server->clients, new_client);
}

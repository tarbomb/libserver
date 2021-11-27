/*
 * Implementations of the client array and methods.
*/

#include <poll.h>
#include <unistd.h>

#define CT_ARRAY_STACK_ONLY

#include "client.h"
#include "structures/array.h"

/* Client array operations */
ct_array_define_append(LibserverClientArray, struct pollfd, libserver_client)

ct_array_define_init(LibserverClientArray, struct pollfd, libserver_client)

ct_array_define_free(LibserverClientArray, libserver_client, libserver_client_free)

void libserver_client_free(struct pollfd value) {
    close(value.fd);
}

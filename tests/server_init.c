/*
 * Tests for initializing the server.
*/

#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "../src/server.h"

int check_error(const char *expected, void (*callback)(const char *file)) {
    char error_message[LIB_SERVER_TESTING_ERROR_LENGTH] = {0};

    libserver_extract_error(LIB_SERVER_TESTING_ERROR_LENGTH, error_message, callback);

    return strcmp(expected, error_message) == 0;
}

void error_null_file() {
    struct LibServerServer new_server = libserver_init(NULL);
}

int main(void) {
    struct LibServerServer new_server = libserver_init("./file");

    assert(check_error("libserver_init: attempt to map shared mutex to NULL pointer\n", error_null_file) == 1);

    return 0;
}

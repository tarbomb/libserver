/*
 * Tests for initializing the server.
*/

#include <assert.h>
#include "../src/server.h"

int main(void) {
    struct LibServerServer new_server = libserver_init("./file");
}

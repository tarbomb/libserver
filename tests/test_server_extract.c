#include <string.h>

#include "common.h"

int main(void) {
    char buffer[256] = {0};

    assert(libserver_server_extract_command("foo bar baz", 256, buffer) == 3);
    assert(strcmp(buffer, "foo") == 0);
    memset(buffer, 0, 256);

    assert(libserver_server_extract_command("foo-bar-baz bar baz", 256, buffer) == 11);
    assert(strcmp(buffer, "foo-bar-baz") == 0);
    memset(buffer, 0, 256);

    assert(libserver_server_extract_command("foo_bar_baz bar baz", 256, buffer) == 11);
    assert(strcmp(buffer, "foo_bar_baz") == 0);
    memset(buffer, 0, 256);

    assert(libserver_server_extract_command("foo_bar_baz bar baz", 4, buffer) == 4);
    assert(strcmp(buffer, "foo_") == 0);
    memset(buffer, 0, 256);

    assert(libserver_server_extract_command("foo-bar-baz bar baz", 0, buffer) == 0);
    assert(strcmp(buffer, "") == 0);
    memset(buffer, 0, 256);

    assert(libserver_server_extract_command("", 256, buffer) == 0);
    assert(strcmp(buffer, "") == 0);
    memset(buffer, 0, 256);

    return EXIT_SUCCESS;
}

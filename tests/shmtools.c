#include "../src/shm-tools/shm-tools.h"

int main(void) {
    shmtools_get_id("./foobar", 12);

    return 0;
}

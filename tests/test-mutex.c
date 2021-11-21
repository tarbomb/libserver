#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <pthread.h>

int main(void) {
    key_t memory_key = ftok("./mutex", 0);
    pthread_mutex_t *process_mutex = NULL;
    int mutex_id = shmget(memory_key, sizeof(pthread_mutex_t), 0644 | IPC_CREAT);
    
    process_mutex = shmat(mutex_id, NULL, 0);
    printf("%p\n", process_mutex);
    pthread_mutex_lock(process_mutex);

    printf("%s", "Mutex is locked by process B\n");

    return EXIT_SUCCESS;
}

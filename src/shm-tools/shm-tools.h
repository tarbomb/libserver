/*
 * Prototypes and structures for shm-tools
*/

#ifndef SHM_TOOLS_H
#define SHM_TOOLS_H

#include <stdlib.h>

#define SHM_TOOLS_EXIT_CODE EXIT_FAILURE

/*
 * Retrieves the memory id a provided file. The program
 * is exited, and an error message is displayed if the
 * file does not exist.
 *
 * @param file: the file to get the shared memory id of
 * @param size: the size of the segment to retrieve
 * @return: the id of the file
*/
int shmtools_get_id(const char *file, size_t size);

/*
 * Like shmtools_get_id, but instead of erroring when
 * the file provided does not exist, the file is created,
 * and then the id of that file is retrieved.
 *
 * @param file: the file to get the shared memory id of
 * @param size: the size of the segment to retrieve
 * @return: the id of the file
*/
int shmtools_get_id_create(const char *file, size_t size);

/*
 * 'Attaches' to the block of shared memory associated with
 * the provided id, and returns a pointer to the data. If the
 * shared memory block could not be attached, an error message
 * is displayed and the program exits.
 *
 * @param id: the block of shared memeory to attach to
 * @return: pointer to the shared memory
*/
void *shmtools_attach(int id);

/*
 * 'Detaches' from the block of shared memory associated with
 * the provided address. If the shared memory block could not be
 * attached, an error message is displayed and the program exits.
 *
 * @param block: address to the block of shared memory
*/
void shmtools_detach(void *block);

/*
 * Destroys the block of shared memory associated with the
 * provided id. If the shared memory block could not be
 * destroyed, an error message is displayed and the program
 * exits.
 *
 * @param id: the block of shared memeory to attach to
*/
void shmtools_destroy(int id);

#endif

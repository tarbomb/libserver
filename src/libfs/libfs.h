#ifndef LIB_FILESYSTEM_H
#define LIB_FILESYSTEM_H

#include <stdlib.h>
#include <dirent.h>

/* Misc */
#define LIBFS_FAILIURE_EXIT_CODE EXIT_FAILURE

/* File types */
#define LIBFS_FILETYPE_BLOCK        0
#define LIBFS_FILETYPE_CHARACTER    1
#define LIBFS_FILETYPE_DIRECTORY    2
#define LIBFS_FILETYPE_FIFO         3
#define LIBFS_FILETYPE_SYMLINK      4
#define LIBFS_FILETYPE_REGULAR      5
#define LIBFS_FILETYPE_SOCKET       6
#define LIBFS_FILETYPE_UNKNOWN      7

/* Limits */
#define LIBFS_IS_REFERENCE_NAME_LENGTH  256

/*
 * Recursively applys a function to a directory tree.
 *
 * @param path: the path to apply to
 * @param callback: the function to invoke for each node
*/
void libfs_recursive_map(const char *path, void (*callback)(const char *path));

/* Information routines */

/*
 * Returns the type of file pointed to by the path PATH.
 *
 * @param path: the 
*/
int libfs_file_type(const char *path);

/*
 * Returns whether or not the file, directory, etc pointed
 * to by PATH exists on the file system.
 *
 * @param path: the path to check
 * @return: 1 if it exists, 0 if it does not exist
*/
int libfs_path_exists(const char *path);

/*
 * Retrieves the last portion of the path PATH, and writes it
 * to a buffer BUFFER that has a length LENGTH.
 *
 * @param path: the path to retrieve the name of
 * @param length: the length of the buffer
 * @param buffer: the buffer to write to
 * @return the new buffer
*/
char *libfs_path_get_name(const char *path, size_t length, char buffer[]);

/*
 * Returns whether or not the path pointed to by PATH is a
 * reference to the parent directory or the current directory,
 * meaning that the directories '..' and '.' are considered
 * references. This does not include symlinks, however.
 *
 * @param path: the path to check
 * @return: 1 if it is a reference, 0 if it is not a reference
*/
int libfs_path_is_reference(const char *path);










#endif

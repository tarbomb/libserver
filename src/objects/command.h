#ifndef LIB_SERVER_COMMAND_H
#define LIB_SERVER_COMMAND_H

struct LibserverServer;
typedef void (*LibseverCallback)(struct LibserverServer *server, int descriptor, const char *arguments);

/*
 * Represents a command that the server can execute.
*/
struct LibserverCommand {
    const char *command;
    LibseverCallback callback;
};

/*
 * Array of commands that are registered into a server.
*/
struct LibserverCommandArray {
    unsigned int logical_size;
    unsigned int physical_size;
    struct LibserverCommand *contents;
};

/*
 * Releases a libserver command from memory.
 *
 * @param value: the value to release
 * @return: an unmodified structure
*/
struct LibserverCommand libserver_command_free(struct LibserverCommand value);

/*
 * Compares two commands by their names.
 *
 * @param compare_a: the first command to compare
 * @param compare_b: the second command to compare
 * @return: whether or not the commands are the same
*/
int libserver_command_compare(struct LibserverCommand compare_a, struct LibserverCommand compare_b);

/*
 * Appends a libserver command onto the array of commands
 * that the server has.
 *
 * @param array: the array to append to
 * @param value: the value to append
*/
void libserver_command_array_append(struct LibserverCommandArray *array, struct LibserverCommand value);

/*
 * Finds the first occurrence of a command in a command array.
 *
 * @param array: the array to search through
 * @param value: the value to find
 * @return: the index of the value, or -1
*/
int libserver_command_array_find(struct LibserverCommandArray *array, struct LibserverCommand value);

/*
 * Initialize a new command array with a maximum size SIZE, and
 * a stack array.
 *
 * @param size: the maximum length of the array
 * @param block: the stack array to use
*/
struct LibserverCommandArray libserver_command_array_init(unsigned int size, struct LibserverCommand *block);

/*
 * Releases a command array from memory.
 *
 * @param array: the array to release
*/
void libserver_command_array_free(struct LibserverCommandArray *array);

#endif

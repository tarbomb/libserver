# libserver
A small C library for handling 'servers.'

# Description
As stated above, libserver is a C library for creating server-like interfaces between
a server program, and a collection of clients. It provides abstractions over sockets
to allow for non-blocking IPC between the clients and the server, and includes its own
command registration system to bind prefixed commands like 'kill' to callbacks that
are executed when the server reads them.
</br>
libsocket tries to do as much on the stack as it possibly can, with the exception of
a shared-memory mutex that comes with the library, which can be used for synchronizing
sockets and process communication.

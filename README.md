# libserver
A small C library for handling 'servers.'

# Description
libserver is a C library for creating socket servers and clients easily, with the intended
purpose of being used for local code such as daemons. It uses the BSD socket interfaces, which
is widely portable to \*NIX and UNIX-like operating systems, with the major exception being Windows.

libserver is designed to be entirely singled threaded, with ANSI C compliance in mind. It uses the
poll system call to allow for non-blocking code to make libserver more flexible for situations where
blocking code is simply not desirable.

# Features
libserver's biggest feature is its ability to assign callbacks when data is received from a client
socket, which makes it easy to create commands for a dedicated daemon or for quick socket program.

libserver is also entirely stack based, with the exception of an (optional) shared-memory mutex
which is intended to be used for callbacks when synchronizing data sent between cliet and server.
This feature can be accessed by programs using this library be assigning the `LIB_SERVER_ENABLE_MUTEX`
macro to 1 in the header file.

Because of this stack-based limitation, libserver uses pre-defined buffer sizes for recieving messages.
However, this limitation can be avoided by simply recieving blocks and processing data one block at a
time.

There is also a place or two in the source code where there are no bounds checks for functions that
write to buffers, like with vsprintf. This will be changed soon.

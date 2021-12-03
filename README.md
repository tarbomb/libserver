# libserver
A small C library for handling 'servers.'

# Description
libserver is a C library for creating socket servers and clients easily, with the intended
purpose of being used for local code such as daemons. It uses the BSD socket interfaces, which
is widly portable to \*NIX and UNIX-like operating systems, with the major exception being Windows.

libserver is designed to be entirely singled threaded, with ANSI C compliance in mind.

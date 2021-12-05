#!/bin/sh

# A bash script to get inclusions to system headers from C source code.
# They are retrieved from both source and header files.

# $1 - the directory to scan for files to search through

# Get from source files
for file in `find "$1" -type f -name '*.c'`; do
    cat "$file" | grep '^#include\s*<.\+>'
done

# Get from header files
for file in `find "$1" -type f -name '*.h'`; do
    cat "$file" | grep '^#include\s*<.\+>'
done

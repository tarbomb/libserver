#!/bin/sh

# A script to get the used functions in C source code.
# The script extracts defines from each header file in
# the directory of $1, and uses it to process them.
# The final files are also sent through the C preprocessor,
# but with inclusions stripped.

# $1 - the directory to collect all source code from
# $2 - the directory which contains the project's code

INCLUDE_GUARD_PATTERN="^#define [a-zA-Z_][a-zA-Z0-9_]*_H"
TEMP_FILE='/tmp/get_used_functions.tmp'
TEMP_PROJECT_FILE='/tmp/get_used_functions_project.tmp'
PROCESSED_TEMP_FILE="/tmp/processed_get_used_functions.tmp"
CONFIG_TEMP_FILE='/tmp/config_get_used_functions.tmp'
TEMP_DEPENDENCIES_FILE="/tmp/dependencies_get_used_functions.tmp"
PROCESSED_TEMP_PROJECT_FILE="/tmp/processed_get_used_functions_project.tmp"

CSOURCE="/home/$USER/core/bin/bin/csource"

rm -rf "$TEMP_FILE"
rm -rf "$TEMP_PROJECT_FILE"
rm -rf "$PROCESSED_TEMP_FILE"
rm -rf "$CONFIG_TEMP_FILE"
rm -rf "$TEMP_DEPENDENCIES_FILE"
rm -rf "$PROCESSED_TEMP_PROJECT_FILE"

# Collect all defines in headers
for file in `find "$1" -type f -name '*.h'`; do
    cat "$file" | grep -v "$INCLUDE_GUARD_PATTERN" | grep '^#define' >> "$TEMP_FILE";
done

for file in `find "$1" -type f -name '*.h'`; do
    cat "$file" | grep -v "$INCLUDE_GUARD_PATTERN" | grep '^#define' >> "$TEMP_PROJECT_FILE";
done

# Collect all source files into a single source file
for file in `find "$1" -type f -name '*.c'`; do
    cat "$file" | $CSOURCE strip-comments | grep -v '^#include' >> "$TEMP_FILE"
done

# Collect all of this project's functions into a single source file
for file in `find $2 -type f -name '*.c'`; do
    cat "$file" | $CSOURCE strip-comments | grep -v '^#include' >> "$TEMP_PROJECT_FILE"
done

# Process the final product with the C Preprocessor
cat "$TEMP_FILE" | cpp | grep -v '^#' | grep -v '^$' | tr --delete '\n' | $CSOURCE separate > "$PROCESSED_TEMP_FILE"
cat "$TEMP_PROJECT_FILE" | cpp | grep -v '^#' | grep -v '^$' | tr --delete '\n' | $CSOURCE separate >> "$PROCESSED_TEMP_PROJECT_FILE"

# Find dependencies of each function in TEMP_PROJECT_FILE.
for function_name in `cat "$PROCESSED_TEMP_PROJECT_FILE" | awk -F'(' '{ print $1 }' | sed 's/\*/ /g' | rev | awk '{ print $1 '} | rev`; do
    cat "$PROCESSED_TEMP_FILE" | $CSOURCE extract-functions | $CSOURCE dependencies "$function_name" >> "$TEMP_DEPENDENCIES_FILE"
done

# Output the source code of each dependency
for function_name in `cat "$TEMP_DEPENDENCIES_FILE" | awk '{ print length " " $0 }' | sort -n | uniq | cut -d' ' -f 2-`; do
    cat "$PROCESSED_TEMP_FILE" | grep "^[A-Za-z_].* $function_name"
done

# Cleanup 
rm -rf "$TEMP_FILE"
rm -rf "$TEMP_PROJECT_FILE"
rm -rf "$PROCESSED_TEMP_FILE"
rm -rf "$CONFIG_TEMP_FILE"
rm -rf "$TEMP_DEPENDENCIES_FILE"
rm -rf "$PROCESSED_TEMP_PROJECT_FILE"

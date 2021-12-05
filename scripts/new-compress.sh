# Construct a pair of files. A header, and source file.
# The source file contains the *used* functions from all
# libraries that are used in this program.

FILE=`realpath "$0"`
PARENT=`dirname "$file"`
ROOT=`dirname "$parent"`

# Useful regular expressions defined
FUNCTION_SIGNATURE='/\*\s*\n(\s*\*\s*.+\n*)*\*/\n*.+;' # pcregrep -M

# Space-delimited list of the headers that contains all compile-time
# constants that are used by the program. This is a specified
# file due to all preprocessor directives that are not related
# to condition compilation being stripped by this compressor.
# All headers are automatically cat'd into a single translation
# unit, and then cat'd to the top of the source file before being
# processed by the C preprocessor.
CONFIG_HEADER="$ROOT/src/objects/config.h $ROOT/src/libsocket/libsocket.h $ROOT/src/shm-tools/shm-tools.h"

# Pattern for identifying include guards that should be stripped.
INCLUDE_GUARD_PATTERN="^\(#define\|#ifndef\).\{1,\}_H"

# The name of the library. Will be used to create the compression
# directory, include guards, filenames, etc.
LIBRARY_NAME="libserver"

# The directory to scan for source and header files.
SRC_DIRECTORY="$ROOT/src"

# The directory of the library-specific source and header files
PROJECT_DIRECTORY="$ROOT/src/objects"

# Outputs all of the compile-time configuration constants.
function compressor_make_configs() {
    for header in $CONFIG_HEADER; do
        cat "$header"              |
        csource strip-comments     |
        grep '^#define'            |
        grep -v "$INCLUDE_GUARD_PATTERN"
    done
}

# Collect all system header inclusions from all source files.
function compressor_collect_inclusions() {
    for source_file in `find "$SRC_DIRECTORY" -type f -name '*.c'`; do
        cat "$source_file"              |
            grep '^#include\s*<'        |
            awk '{print length " " $0}' >> "$ROOT/$LIBRARY_NAME/sort_intermediary"

    done

    cat "$ROOT/$LIBRARY_NAME/sort_intermediary" | sort | cut -d ' ' -f 2- | uniq
    rm "$ROOT/$LIBRARY_NAME/sort_intermediary"
}

# Collects all C source files recursively from a directory
# into a single file with inclusions stripped, leaving only
# structures, enumerations, function bodies, etc.
function compressor_collect_source() {
    for source_file in `find "$1" -type f -name '*.c'`; do
        cat "$source_file" | csource strip-comments | grep -v '^#include'
    done
}

# Collects all C header files recursively from a directory
# into a single file with inclusions stripped, leaving only
# structures, enumerations, function signatures, and directives
# related to conditional compilation.
function compressor_collect_headers() {
    for header_file in `find "$1" -type f -name '*.h'`; do
        cat "$header_file" | grep -v "$INCLUDE_GUARD_PATTERN" | tac | sed -n '2,$p' | tac | grep -v '^\(#include\|#define\)' | grep -v '^$'
    done
}


# Invokes the C preprocessor on the file, and performs other operations like
# folding each function onto its own line.
function compressor_process() {
    cpp "$1"             |
        grep -v '^#'     |
        tr --delete '\n' |
        csource separate > "$2"
}

function compressor_process_header() {
    cpp $1 -C |
        grep -v '^#' | grep -v '^$'
}

# Extracts the names of functions that are used by in a file.
function compressor_extract_function_names() {
    cat "$1"                     |
        grep -v '^#include'      |
        awk -F'(' '{ print $1 }' |
        sed 's/\*/ /g'           |
        rev                      |
        awk '{ print $1 }'       |
        rev
}

# Extracts the dependencies of a function
function compressor_function_dependencies() {
    cat $1 | csource extract-functions | csource dependencies $2
}

# Generate the source file's functions
function compressor_generate_source_functions() {
    for source_function in `compressor_extract_function_names $2`; do
        dependency_tree=`compressor_function_dependencies $1 $source_function`

        for dependency in $dependency_tree; do
            cat $1 | grep "$dependency(.*)\s*{"
        done
    done
}

function compressor_remove_duplicates() {
    cat $1 | awk '{ print length " " $0 }' | sort -n | cut -d' ' -f 2- | uniq
}

# Retrieves the function signatures (comments, and protoypes) for each function
# that is usd by the library.
function compressor_get_signatures() {
    for function in `compressor_extract_function_names $1`; do
        cat $2 | pcregrep -M "/\*\s*\n(\s*\*\s*.+\n)*\*/\n.+$function.+;"
    done
}

# Retrieves the structures defined in a header file.
function compressor_get_structures() {
    cat $1 | pcregrep -M '/\*\s*\n(\s*\*\s*.+\n)*\*/\n.+\s*{\n*(.+;\n?)*};'
}

# Retrieves the typedefs defined in a header file.
function compressor_get_typedefs() {
    cat $1 | pcregrep -M '/\*\s*\n(\s*\*\s*.+\n)*\*/\n^typedef .+;\n'
}

# Recursively retrieves each forward declaration from every header file
# in a directory.
function compressor_get_forward_declarations() {
        cat `find "$1" -type f -name '*.h'` | grep '^\(struct\|enum\) [^(]\+;'
}

# Preprocessing
rm -rf "$ROOT/$LIBRARY_NAME"
mkdir "$ROOT/$LIBRARY_NAME"

compressor_collect_inclusions >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
compressor_make_configs >> "$ROOT/$LIBRARY_NAME/functions"
compressor_make_configs >> "$ROOT/$LIBRARY_NAME/project_functions"
compressor_make_configs >> "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.h"

compressor_collect_source "$SRC_DIRECTORY" >> "$ROOT/$LIBRARY_NAME/functions"
compressor_collect_source "$PROJECT_DIRECTORY" >> "$ROOT/$LIBRARY_NAME/project_functions"

compressor_process "$ROOT/$LIBRARY_NAME/functions" "$ROOT/$LIBRARY_NAME/processed_functions"
compressor_process "$ROOT/$LIBRARY_NAME/project_functions" "$ROOT/$LIBRARY_NAME/processed_project_functions"

echo "#include\"$LIBRARY_NAME.h\"" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c"
compressor_generate_source_functions "$ROOT/$LIBRARY_NAME/processed_functions" "$ROOT/$LIBRARY_NAME/processed_project_functions" >> "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.c"
compressor_remove_duplicates "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.c" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c"

compressor_collect_headers "$ROOT/src" >> "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.h"
compressor_process_header "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.h" >> "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h"

compressor_get_forward_declarations "$SRC_DIRECTORY" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
compressor_get_typedefs "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
compressor_get_structures "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
compressor_get_signatures "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c" "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

mv "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h" '.'
mv "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c" '.'

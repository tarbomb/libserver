FILE=`realpath "$0"`
PARENT=`dirname "$file"`
ROOT=`dirname "$parent"`

# The basename
LIBRARY_NAME="libserver"

# The name of the include guard for the header
INCLUDE_GUARD="LIBSERVER_H"

# The first common directory of each source and header file.
SRC_DIRECTORY="$ROOT/src"
PROJ_DIRECTORY="$ROOT/src/objects"

# The location of headers which contain important compile-time constants,
# which will be grep'd for defines and merged into the final file.
CONFIG_HEADERS="$SRC_DIRECTORY/libsocket/libsocket.h
                $SRC_DIRECTORY/shm-tools/shm-tools.h
                $SRC_DIRECTORY/objects/config.h"

# Regular expression components
RE_MULTILINE_COMMENT='/\*\s*\n?(\s*\*.*\n?)*\*/'
RE_IDENTIFIER='[a-zA-Z_][a-zA-Z0-9_]*'

# Regular expressions
RE_SYSTEM_HEADER='^#include <.\+>'
RE_STRUCTURE="($RE_MULTILINE_COMMENT\nstruct $RE_IDENTIFIER\s*{\n?)(\s*.+;\n?)*};\n?"
RE_FORWARD_DECL="(struct|enum)\s*$RE_IDENTIFIER;"
RE_TYPEDEF="$RE_MULTILINE_COMMENT\ntypedef .+;"
RE_INCLUDE_GUARD="${RE_IDENTIFIER}_H"

function get_defines() {
    for file in `find "$1" -type f -name '*.h'`; do
        cat "$file" | grep '^#define' | grep -v "$RE_INCLUDE_GUARD"
    done
}

function cat_headers() {
    cat `find "$1" -type f -name '*.h'` 
}

# Preprocessing
rm -rf "$ROOT/$LIBRARY_NAME"
mkdir "$ROOT/$LIBRARY_NAME"

# Make the source file
echo "#include \"$LIBRARY_NAME.h\"" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c"
"$ROOT/scripts/get_used_functions.sh" "$SRC_DIRECTORY" "$PROJ_DIRECTORY" | awk '{ print length " " $0 }' | sort -n | cut -d ' ' -f 2- | uniq >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c"

# Make the unprocessed header file
get_defines "$SRC_DIRECTORY" >> "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.h"
get_defines "$SRC_DIRECTORY" >> "$ROOT/$LIBRARY_NAME/config.h"

# Dump the contents of each header with inclusions, guards, and defines stripped.
for header_file in `find "$SRC_DIRECTORY" -type f -name '*.h'`; do
    cat "$header_file" | grep -v "$RE_INCLUDE_GUARD" | tac | sed -n '2,$p' | tac | grep -v '^#\(include\|define\)' >> "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.h"
done

# Process with CPP
cpp -C "$ROOT/$LIBRARY_NAME/unprocessed_$LIBRARY_NAME.h" | grep -v '^\(#\|$\)' >> "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h"

# Prepare the final header
echo "#ifndef $INCLUDE_GUARD" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
echo "#define $INCLUDE_GUARD" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
echo '' >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

"$ROOT/scripts/get_system_inclusions.sh" "$SRC_DIRECTORY" | awk '{ print length " " $0 }' | sort -n | cut -d ' ' -f 2- | uniq >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
echo '' >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

# Extract various tokens into the final header
cat "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" | pcregrep -M "$RE_FORWARD_DECL" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
echo '' >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

cat "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" | pcregrep -M "$RE_TYPEDEF" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
cat "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" | pcregrep -M "$RE_STRUCTURE" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

# Extract all used function signatures into the final header
for function_name in `cat "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.c" | grep -v '^#' | sed 's/\*/ /g' |sed 's/{.*}//g' | awk -F'(' '{ print $1 }'| rev | awk '{ print $1 }' | rev`; do
    cat "$ROOT/$LIBRARY_NAME/config.h" "$ROOT/$LIBRARY_NAME/processed_$LIBRARY_NAME.h" | grep -v '^#include' | cpp -C -nostdinc | pcregrep -M "$RE_MULTILINE_COMMENT\n.+$function_name\(" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
done

echo "#endif" >> "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

# Tidy up the final header
sed -i 's/};/};\n/g' "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"
sed -i 's/);/);\n/g' "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME.h"

# Cleanup
mv "$ROOT/$LIBRARY_NAME/$LIBRARY_NAME".* "$ROOT"
rm "$ROOT/$LIBRARY_NAME/"*
mv "$ROOT/$LIBRARY_NAME."* "$ROOT/$LIBRARY_NAME"

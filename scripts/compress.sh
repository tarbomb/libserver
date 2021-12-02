library=libserver
file=`realpath "$0"`
parent=`dirname "$file"`
root=`dirname "$parent"`

rm -rf "$root/$library"

# Make a directory for the compressed library
mkdir --parents "$root/$library"

# Extract the source code of each used function
cat `find "$root/src" -type f -name '*.c'` | csource strip-comments | grep -v ^# | sed 's/^\s\{2,\}//g' | grep -v ^$ | tr --delete '\n' | csource separate > "$root/$library/source_functions.c"

# Get each function that this library implements
for function_name in `cat src/objects/*.c | csource strip-comments | grep -v '^#'| sed 's/^\s\{1,\}//g' | grep -v ^$ | tr --delete '\n' | csource separate | sed 's/\*/ /g' | awk -F'(' '{ print $1 }' | rev | awk '{ print $1 }' | rev`; do
    cat "$root/$library/source_functions.c" | csource extract-functions | csource dependencies "$function_name" | awk '{ print length " " $1 }' | sort -n | awk '{ print $2 }' | uniq >> "$root/$library/dependencies.c"
done

# Filter out duplicate dependencies
cat "$root/$library/dependencies.c" | awk '{ print length " " $0 }' | sort -n | cut -d' ' -f 2- | uniq > "$root/$library/dependencies.c"

echo -n '#include' >> "$root/$library/libserver.c"
echo \"libserver.h\" >> "$root/$library/libserver.c"

# Extract the source from each function into a single source file
for function_name in `cat "$root/$library/dependencies.c"`; do
    cat "$root/$library/source_functions.c" | grep ".\{2,\}$function_name(.*)\s*{" | awk '{ print length " " $0 }' >> "$root/$library/libserver.c"
done

# Filter out duplicate dependency code
cat "$root/$library/libserver.c" | sort -n | cut -d' ' -f 1- | uniq | cut -d' ' -f 2- > "$root/$library/libserver.c"

# Create header file
cat << EOF > "$root/$library/$library.h"
#ifndef LIB_SERVER_H
#define LIB_SERVER_H
EOF

# Extract all compile-time constants
cat `find src/ -type f | grep .h$` | grep ^#define | grep -v '#define .\{1,\}_H' >> "$root/$library/$library.h"

# Extract all included system headers
cat `find "$root/src/" -type f -name '*.c'` | grep '^#.\{1,\}<' | sed 's/#include /#include/g' | awk '{ print length " " $1 }' | sort -n | uniq | awk '{ print $2 }' >> "$root/$library/$library.h"

# Extract forward declarations
cat `find src/ -type f -name '*.h'` | grep '^struct [a-zA-Z0-9][a-zA-Z0-9_]\{1,\};' >> "$root/$library/$library.h"

# Extract all structures
cat `find src/ -type f -name '*.h'` | csource strip-comments | grep -v ^$ | grep -v ^# | grep '^\(struct .\{1,\}{\|    \|};\|typedef\)' >> "$root/$library/$library.h"

# Extract functions
cat libserver/libserver.c | grep -v ^# | sed 's/{.\{1,\}}/{}/g' | sed 's/\s*{}/;/g' >> "$root/$library/$library.h"

echo '#endif' >> "$root/$library/$library.h"

rm "$root/$library/dependencies.c"
rm "$root/$library/source_functions.c"

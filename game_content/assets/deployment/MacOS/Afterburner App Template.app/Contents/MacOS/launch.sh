#! /bin/sh

# Ensure that the dylib environment variable is set when running the executable.
EXE_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
DYLD_LIBRARY_PATH=$EXE_DIR $EXE_DIR/bond $@

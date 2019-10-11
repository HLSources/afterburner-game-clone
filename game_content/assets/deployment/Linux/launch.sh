#! /bin/sh

# Ensure that the lib environment variable is set when running the executable.
EXE_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
LD_LIBRARY_PATH=$EXE_DIR $EXE_DIR/bond $@

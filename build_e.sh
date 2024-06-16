#!/bin/bash

# Code for executable building
# Usage:
# chmod +x build_e.sh
# ./build_e.sh <file_name_souce_code> <executable_name>

# Temporary file for assembly code
ASSEMBLY_FILE=$(mktemp /tmp/output.asm.XXXXXX)
OBJECT_FILE=$(mktemp /tmp/output.o.XXXXXX)

# Function to clean tmp file in case of error
cleanup() {
    rm -f "$ASSEMBLY_FILE" "$OBJECT_FILE"
}
# Save function for error
trap cleanup EXIT

# Compiler
COMPILER=easycomp

# Read the file
SOURCE_FILENAME="$1"
EXECUTABLE_NAME="$2"

# Compile the source code and assemble the output
./easycomp $SOURCE_FILENAME > $ASSEMBLY_FILE
nasm -f elf64 -o $OBJECT_FILE $ASSEMBLY_FILE

# Remove assembly file
rm -f "$ASSEMBLY_FILE"

# Linking
ld -o $EXECUTABLE_NAME $OBJECT_FILE -lc --dynamic-linker=/lib64/ld-linux-x86-64.so.2 

# Remove object file
rm -f "$OBJECT_FILE"
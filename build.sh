#!/bin/bash

# Code for compiler and interpretr building
# Usage:
# chmod +x build.sh
# ./build.sh

gcc easy_compiler.c -o easycomp -Wall -pedantic -O3
gcc easy_interpreter.c -o easyint -Wall -pedantic -O3
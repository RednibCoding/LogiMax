#!/bin/bash

# Simple build script template for small projects
# No need for 'make' and stuff

echo "Starting compilation..."

# Define compiler
COMPILER=gcc

# Output file name
OUTPUT_FILE_NAME=main

# Define resource files to include
RES_FILES=

# Define the source files to include
SRC_FILES=main.c node.c connector.c

# Debug build
# Uncomment the following line for debug build
# $COMPILER $SRC_FILES $RES_FILES -o $OUTPUT_FILE_NAME -std=c99 -lGLU -lGL -lX11

# Release build
$COMPILER $SRC_FILES $RES_FILES -o $OUTPUT_FILE_NAME -std=c99 -s -lGLU -lGL -lX11 -O3 -march=native -funroll-loops -flto -fomit-frame-pointer

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit $?
else
    echo "Compilation succeeded."
fi

# Check if the executable exists before trying to execute it
if [ -f "./$OUTPUT_FILE_NAME" ]; then
    echo "Executing $OUTPUT_FILE_NAME..."
    ./$OUTPUT_FILE_NAME
else
    echo "Executable $OUTPUT_FILE_NAME not found."
fi

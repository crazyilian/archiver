#!/bin/bash

# Run from build directory

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 task-name"
    exit 1
fi

TASK_PATH=../src/$1
CLANG_PATH=../run-clang-format.py

if [ ! -f compile_commands.json ]; then
    echo "Run this script from the build directory"
    exit 1
fi

if [ "$#" -eq 2 ]; then
    TASK_PATH=../../src/$1
    CLANG_PATH=../../run-clang-format.py
fi

$CLANG_PATH -r $TASK_PATH && clang-tidy-11 $TASK_PATH/*.cpp

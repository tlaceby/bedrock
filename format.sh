#!/bin/bash

# Specify the directory containing the source files
SOURCE_DIR="src"

# Check if the source directory exists
if [ ! -d "$SOURCE_DIR" ]; then
  echo "The directory $SOURCE_DIR does not exist."
  exit 1
fi

# Navigate to the source directory
cd "$SOURCE_DIR"

# Find all .h, .c, and .cpp files and format them using clang-format
find . \( -name '*.h' -o -name '*.c' -o -name '*.cpp' \) -exec clang-format -i {} \;

# Return to the original directory
cd -

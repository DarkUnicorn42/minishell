#!/bin/bash

# Name of the output file
OUTPUT_FILE="all_code.txt"

# Remove the output file if it already exists
rm -f "$OUTPUT_FILE"

# Find and concatenate all .c and .h files
find . -type f \( -name "*.c" -o -name "*.h" \) -exec cat {} + >> "$OUTPUT_FILE"

echo "All code has been collected into $OUTPUT_FILE"
#!/bin/bash

# This is a sample bash script that creates a filmstrip from a list of input files.
# usage: ./make_filmstrip.sh -i input1.png input2.png input3.png -o filmstrip.png
# -i: list of input files
# -o: output file
# -s: stack type (hstack or vstack for horizontal or vertical stacking)

# Generate the list of input files
input_files=()
output_file="filmstrip.png"
stack="vstack"

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -i)
            shift
            while [[ "$#" -gt 0 && "$1" != -* ]]; do
                input_files+=("$1")
                shift
            done
            ;;
        -o)
            shift
            output_file="$1"
            shift
            ;;
        -s)
            shift
            stack="$1"
            shift
            ;;  
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

echo "Input files: $input_files"

# Create the inputs array from the input_files list
for file in "${input_files[@]}"; do
    inputs+=("-i $file")
done

echo "Inputs: ${inputs[@]}"

num_inputs=${#inputs[@]}

echo "Number of input files: $num_inputs"

if [ "$num_inputs" -lt 2 ]; then
    echo "Warning: At least two input files are required to create a filmstrip."
    exit 1
fi

echo "Creating a filmstrip from $num_inputs input files."

# Generate the filter_complex string
filter_complex="[0"
for ((i=1; i<num_inputs; i++)); do
    filter_complex+="][$i"
done
filter_complex+="]$stack=inputs=$num_inputs"

# Construct and run the ffmpeg command
cmd=("ffmpeg ${inputs[@]} -filter_complex \"$filter_complex\" $output_file")

# echo "$cmd"
eval "$cmd"

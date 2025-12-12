#!/bin/bash

# aoc.sh - A script to build, delete, and finish Advent of Code projects.

# Return 0 (true) if the given year/day has a Part 2.
aoc_has_part2() {
    local yyyy=$1
    local nn=$2

    # Day 25 never has a Part 2 in AoC.
    if [[ "${nn}" == "25" ]]; then
        return 1
    fi

    # 2025 special-case: only 12 days total; day 12 has no Part 2.
    if [[ "${yyyy}" == "2025" && "${nn}" == "12" ]]; then
        return 1
    fi

    return 0
}

aoc_max_day_for_year() {
    local yyyy=$1
    if [[ "${yyyy}" == "2025" ]]; then
        echo "12"
    else
        echo "25"
    fi
}

# Function to build the binary
aocbuild() {
    local param=$1

    # Extract the folder name, assuming the script is called from within the Dnn directory
    local folder_name
    folder_name=$(basename "$PWD")

    # Extract the numeric value from the folder name (e.g., "21" from "D21")
    if [[ $folder_name =~ ^D([0-9]+)$ ]]; then
        local n=${BASH_REMATCH[1]}
    else
        echo "Error: Current directory name must be in the format D<number> (e.g., D21)"
        exit 1
    fi

    # Check if a parameter is passed and is numeric
    if [[ -z $param ]]; then
        echo "Usage: $0 build <parameter>"
        echo "Parameter should be a numeric value, e.g., 1 or 2."
        exit 1
    elif ! [[ $param =~ ^[0-9]+$ ]]; then
        echo "Error: Parameter must be a numeric value."
        exit 1
    fi

    # Guard against trying to build a non-existent Part 2 (AoC day/year special-cases).
    if [[ "${param}" == "2" ]]; then
        local parent_folder
        parent_folder=$(basename "$(dirname "$PWD")")
        if [[ "${parent_folder}" =~ ^[0-9]{4}$ ]]; then
            # Normalize day number to 2 digits for comparisons (e.g., D1 -> 01).
            local nn
            nn=$(printf "%02d" "$n")
            if ! aoc_has_part2 "${parent_folder}" "${nn}"; then
                echo "Error: ${parent_folder} day ${nn} does not have a Part 2."
                exit 1
            fi
        fi
    fi

    # Compile using g++
    local output_file="d${n}p${param}"
    local source_file="${output_file}.cpp"

    if [[ ! -f $source_file ]]; then
        echo "Error: Source file '$source_file' does not exist in $(pwd)."
        exit 1
    fi

    g++ -O2 -std=c++2b -o "$output_file" "$source_file"

    # Check if the compilation was successful
    if [[ $? -eq 0 ]]; then
        echo "Compiled successfully: $output_file"
    else
        echo "Compilation failed for $source_file"
        exit 1
    fi
}

# Function to delete binaries
aocdelete() {
    # Extract the folder name, assuming the script is called from within the Dnn directory
    local folder_name
    folder_name=$(basename "$PWD")

    # Extract the numeric value from the folder name (e.g., "21" from "D21")
    if [[ $folder_name =~ ^D([0-9]+)$ ]]; then
        local n=${BASH_REMATCH[1]}
    else
        echo "Error: Current directory name must be in the format D<number> (e.g., D21)"
        exit 1
    fi

    # Define the binaries to delete
    local binaries=("d${n}p1" "d${n}p2")

    # Remove the binaries
    rm -f "${binaries[@]}"

    # Check if files were successfully deleted
    if [[ $? -eq 0 ]]; then
        echo "Deleted binaries: ${binaries[*]}"
    else
        echo "No binaries found to delete or deletion failed."
        exit 1
    fi
}

# Function to finish the task
aocfinish() {
    # Extract {nn} from the current folder name (e.g., D21 -> 21)
    local folder_name
    folder_name=$(basename "$PWD")
    local nn
    nn=$(echo "$folder_name" | sed -E 's/^D([0-9]{2})$/\1/')

    # Extract {yyyy} from the parent folder name (e.g., 2024)
    local parent_folder
    parent_folder=$(basename "$(dirname "$PWD")")
    local yyyy="$parent_folder"

    # Ensure we are in the correct folder structure
    if [[ ! "$nn" =~ ^[0-9]{2}$ || ! "$yyyy" =~ ^[0-9]{4}$ ]]; then
        echo "Error: Current directory must be of the form YEAR/Dnn (e.g., 2024/D21)"
        exit 1
    fi

    local max_day
    max_day=$(aoc_max_day_for_year "${yyyy}")
    if (( 10#${nn} > max_day )); then
        echo "Error: ${yyyy} only has ${max_day} day(s); got day ${nn}."
        exit 1
    fi

    # Define input and output files
    local input_file="input-d${nn}.txt"
    local output1="solution-d${nn}p1.txt"
    local output2="solution-d${nn}p2.txt"

    if [[ ! -f $input_file ]]; then
        echo "Error: Input file '$input_file' does not exist in $(pwd)."
        exit 1
    fi

    if ! aoc_has_part2 "${yyyy}" "${nn}"; then
        echo "No Part 2 for ${yyyy} day ${nn}. Only processing Part 1."

        # Build Part 1
        "$SCRIPT_PATH" build 1

        # Execute Part 1
        cat "$input_file" | "./d${nn}p1" > "$output1"
        if [[ $? -eq 0 ]]; then
            echo "Generated $output1"
        else
            echo "Execution failed for d${nn}p1"
            exit 1
        fi
    else
        echo "Processing Day $nn. Building and executing both parts."

        # Build Part 1 and Part 2
        "$SCRIPT_PATH" build 1
        "$SCRIPT_PATH" build 2

        # Execute Part 1
        cat "$input_file" | "./d${nn}p1" > "$output1"
        if [[ $? -eq 0 ]]; then
            echo "Generated $output1"
        else
            echo "Execution failed for d${nn}p1"
            exit 1
        fi

        # Execute Part 2
        cat "$input_file" | "./d${nn}p2" > "$output2"
        if [[ $? -eq 0 ]]; then
            echo "Generated $output2"
        else
            echo "Execution failed for d${nn}p2"
            exit 1
        fi
    fi

    # Delete binaries (both p1 and p2, p2 might not exist for Day 25)
    "$SCRIPT_PATH" delete

    # Git operations: run from the parent directory
    local project_root
    project_root=$(dirname "$SCRIPT_PATH")
    
    (
        cd "$project_root" || exit 1
        git add -A
        git commit -m "Completed ${yyyy}-D${nn}."
        git push
    )

    if [[ $? -eq 0 ]]; then
        echo "Git changes pushed successfully."
    else
        echo "Git push failed."
        exit 1
    fi
}

# Determine the absolute path to the script
SCRIPT_PATH="$(realpath "$0")"

# Main script logic to handle subcommands
if [[ $# -lt 1 ]]; then
    echo "Usage: $0 {build <param>|delete|finish}"
    exit 1
fi

command=$1
shift

case "$command" in
    build)
        aocbuild "$@"
        ;;
    delete)
        aocdelete "$@"
        ;;
    finish)
        aocfinish "$@"
        ;;
    *)
        echo "Invalid command: $command"
        echo "Usage: $0 {build <param>|delete|finish}"
        exit 1
        ;;
esac

# Advent of Code Solutions (C++)
This repository contains my solutions to Advent of Code puzzles for the years 2015-2016 and 2022-2024. Folders and input files exist for other years, but solutions are not yet implemented.

Please note: Input files are encrypted using git-crypt due to Advent of Code's policy against sharing puzzle inputs.

## Building and Running Solutions
The code is written in C++ and uses g++ for compilation. A helper script named aochelpers.sh located in the root directory assists with building and deleting binaries.

### Compiling:

Navigate to the folder for the day of interest and run:

```../../aochelpers.sh build <part_number>  # e.g., ../../aochelpers.sh build 1```

This script assumes you are in a folder named D<day> (e.g., D03) and builds the executable for the specified part (1 or 2) of the day's puzzle.

### Running:
Solutions take input from standard input (stdin).

```./d<day>p<part> < input-d<day>.txt```

For example:

```./d01p1 < input-d01.txt```

This will execute the solution for day 1, part 1 in 2024, using the contents of input-d01.txt as input.

## License
This project is open-source and licensed under the MIT License.



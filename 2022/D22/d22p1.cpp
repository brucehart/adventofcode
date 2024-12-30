#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

// Directions: 0 = right, 1 = down, 2 = left, 3 = up
struct Position {
    int row;
    int col;
    int facing;
};

int main() {
    std::vector<std::string> map;
    std::string line;

    // Read the map
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break; // End of map input
        }
        map.push_back(line);
    }

    // Read the movement instructions
    std::string instructions;
    if (std::getline(std::cin, instructions)) {
        // Do nothing, already read into 'instructions'
    }

    // Determine the maximum row length to pad shorter rows
    size_t max_row_length = 0;
    for (const auto& row : map) {
        if (row.size() > max_row_length) {
            max_row_length = row.size();
        }
    }

    // Pad all rows with spaces to have the same length
    for (auto& row : map) {
        if (row.size() < max_row_length) {
            row += std::string(max_row_length - row.size(), ' ');
        }
    }

    size_t num_rows = map.size();
    size_t num_cols = max_row_length;

    // Precompute min and max columns for each row
    std::vector<int> min_col(num_rows, 0);
    std::vector<int> max_col_indices(num_rows, 0);
    for (size_t r = 0; r < num_rows; ++r) {
        const std::string& row = map[r];
        // Find first non-space character
        auto it = std::find_if(row.begin(), row.end(), [](char c) { return c != ' '; });
        if (it != row.end()) {
            min_col[r] = std::distance(row.begin(), it);
            // Find last non-space character
            auto it_rev = std::find_if(row.rbegin(), row.rend(), [](char c) { return c != ' '; });
            max_col_indices[r] = num_cols - 1 - std::distance(row.rbegin(), it_rev);
        }
    }

    // Precompute min and max rows for each column
    std::vector<int> min_row(num_cols, 0);
    std::vector<int> max_row_indices(num_cols, num_rows - 1);
    for (size_t c = 0; c < num_cols; ++c) {
        // Find first non-space character in column
        int first = 0;
        while (first < static_cast<int>(num_rows) && map[first][c] == ' ') {
            ++first;
        }
        min_row[c] = first;
        // Find last non-space character in column
        int last = num_rows - 1;
        while (last >= 0 && map[last][c] == ' ') {
            --last;
        }
        max_row_indices[c] = last;
    }

    // Find the starting position: first '.' in the top row
    Position pos;
    pos.row = 0;
    pos.col = min_col[0];
    pos.facing = 0; // Initially facing right
    while (map[pos.row][pos.col] != '.') {
        ++pos.col;
        if (pos.col > max_col_indices[pos.row]) {
            // No open tile found
            break;
        }
    }

    // Parse the movement instructions into a vector of instructions
    // Each instruction is either an integer (steps) or a char ('R' or 'L')
    std::vector<std::string> parsed_instructions;
    size_t i = 0;
    while (i < instructions.size()) {
        if (std::isdigit(instructions[i])) {
            std::string number;
            while (i < instructions.size() && std::isdigit(instructions[i])) {
                number += instructions[i];
                ++i;
            }
            parsed_instructions.push_back(number);
        } else if (instructions[i] == 'R' || instructions[i] == 'L') {
            parsed_instructions.push_back(std::string(1, instructions[i]));
            ++i;
        } else {
            // Ignore any other characters (if any)
            ++i;
        }
    }

    // Execute each instruction
    for (const auto& instr : parsed_instructions) {
        if (instr == "R" || instr == "L") {
            // Turn instruction
            if (instr == "R") {
                pos.facing = (pos.facing + 1) % 4;
            } else { // "L"
                pos.facing = (pos.facing + 3) % 4; // Equivalent to -1 modulo 4
            }
        } else {
            // Move instruction
            int steps = std::stoi(instr);
            for (int step = 0; step < steps; ++step) {
                int next_row = pos.row;
                int next_col = pos.col;
                // Determine the next position based on current facing
                switch (pos.facing) {
                    case 0: // Right
                        next_col += 1;
                        if (next_col > max_col_indices[pos.row] || map[pos.row][next_col] == ' ') {
                            // Wrap to min_col of this row
                            next_col = min_col[pos.row];
                        }
                        break;
                    case 1: // Down
                        next_row += 1;
                        if (next_row > max_row_indices[pos.col] || map[next_row][pos.col] == ' ') {
                            // Wrap to min_row of this column
                            next_row = min_row[pos.col];
                        }
                        break;
                    case 2: // Left
                        next_col -= 1;
                        if (next_col < min_col[pos.row] || map[pos.row][next_col] == ' ') {
                            // Wrap to max_col of this row
                            next_col = max_col_indices[pos.row];
                        }
                        break;
                    case 3: // Up
                        next_row -= 1;
                        if (next_row < min_row[pos.col] || map[next_row][pos.col] == ' ') {
                            // Wrap to max_row of this column
                            next_row = max_row_indices[pos.col];
                        }
                        break;
                }

                // Check the tile at the next position
                if (map[next_row][next_col] == '#') {
                    // Wall encountered; stop moving
                    break;
                } else if (map[next_row][next_col] == '.') {
                    // Move to the next position
                    pos.row = next_row;
                    pos.col = next_col;
                } else {
                    // Should not encounter any other characters; treat as wall
                    break;
                }
            }
        }
    }

    // Calculate the final password
    // Rows and columns are 1-based
    int final_password = 1000 * (pos.row + 1) + 4 * (pos.col + 1) + pos.facing;

    // Output the final password
    std::cout << final_password << std::endl;

    return 0;
}

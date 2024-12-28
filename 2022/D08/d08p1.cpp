#include <iostream>
#include <vector>
#include <string>

// Function to read the grid from standard input
std::vector<std::vector<int>> readGrid() {
    std::vector<std::vector<int>> grid;
    std::string line;
    while (std::cin >> line) {
        std::vector<int> row;
        for (char ch : line) {
            // Convert character digit to integer
            row.push_back(ch - '0');
        }
        grid.push_back(row);
    }
    return grid;
}

int main() {
    // Read the grid from stdin
    std::vector<std::vector<int>> grid = readGrid();

    if (grid.empty()) {
        std::cout << 0 << std::endl;
        return 0;
    }

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Initialize a visibility grid with all false
    std::vector<std::vector<bool>> visible(rows, std::vector<bool>(cols, false));

    // Traverse from left to right for each row
    for (int r = 0; r < rows; ++r) {
        int max_height = -1;
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] > max_height) {
                visible[r][c] = true;
                max_height = grid[r][c];
            }
        }
    }

    // Traverse from right to left for each row
    for (int r = 0; r < rows; ++r) {
        int max_height = -1;
        for (int c = cols - 1; c >= 0; --c) {
            if (grid[r][c] > max_height) {
                visible[r][c] = true;
                max_height = grid[r][c];
            }
        }
    }

    // Traverse from top to bottom for each column
    for (int c = 0; c < cols; ++c) {
        int max_height = -1;
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] > max_height) {
                visible[r][c] = true;
                max_height = grid[r][c];
            }
        }
    }

    // Traverse from bottom to top for each column
    for (int c = 0; c < cols; ++c) {
        int max_height = -1;
        for (int r = rows - 1; r >= 0; --r) {
            if (grid[r][c] > max_height) {
                visible[r][c] = true;
                max_height = grid[r][c];
            }
        }
    }

    // Count the number of visible trees
    int visible_count = 0;
    for (const auto& row : visible) {
        for (bool is_visible : row) {
            if (is_visible) {
                ++visible_count;
            }
        }
    }

    // Output the result
    std::cout << visible_count << std::endl;

    return 0;
}


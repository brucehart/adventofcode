#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
        grid.emplace_back(std::move(row));
    }
    return grid;
}

// Function to calculate viewing distance in a specific direction
int viewingDistance(const std::vector<std::vector<int>>& grid, int row, int col, int dRow, int dCol) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    int distance = 0;
    int currentHeight = grid[row][col];
    int r = row + dRow;
    int c = col + dCol;

    while (r >= 0 && r < rows && c >= 0 && c < cols) {
        distance++;
        if (grid[r][c] >= currentHeight) {
            break; // View is blocked
        }
        r += dRow;
        c += dCol;
    }
    return distance;
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

    int max_scenic_score = 0;

    // Iterate over each tree in the grid
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // Calculate viewing distances in all four directions
            int up = viewingDistance(grid, r, c, -1, 0);
            int down = viewingDistance(grid, r, c, 1, 0);
            int left = viewingDistance(grid, r, c, 0, -1);
            int right = viewingDistance(grid, r, c, 0, 1);

            // Calculate scenic score as the product of viewing distances
            int scenic_score = up * down * left * right;

            // Update the maximum scenic score found
            if (scenic_score > max_scenic_score) {
                max_scenic_score = scenic_score;
            }
        }
    }

    // Output the highest scenic score
    std::cout << max_scenic_score << std::endl;

    return 0;
}


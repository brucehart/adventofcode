#include <iostream>
#include <vector>
#include <algorithm>

// Function to count the number of lit neighbors for a given cell
int countLitNeighbors(const std::vector<std::string>& grid, int row, int col) {
    int litNeighbors = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    // Check all adjacent cells
    for (int i = std::max(0, row - 1); i <= std::min(rows - 1, row + 1); ++i) {
        for (int j = std::max(0, col - 1); j <= std::min(cols - 1, col + 1); ++j) {
            if (i == row && j == col) continue; // Skip the cell itself
            if (grid[i][j] == '#') {
                litNeighbors++;
            }
        }
    }
    return litNeighbors;
}

// Function to set the corners to '#' (on) state.
void setCornersOn(std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    grid[0][0] = '#';
    grid[0][cols-1] = '#';
    grid[rows-1][0] = '#';
    grid[rows-1][cols-1] = '#';
}

// Function to perform one step of the animation
std::vector<std::string> performStep(const std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::string> newGrid = grid; // Create a new grid for the next step

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int litNeighbors = countLitNeighbors(grid, row, col);
            // Rules for updating the lights
            if (grid[row][col] == '#') { // If the light is on
                if (litNeighbors != 2 && litNeighbors != 3) {
                    newGrid[row][col] = '.'; // Turn off
                }
            } else { // If the light is off
                if (litNeighbors == 3) {
                    newGrid[row][col] = '#'; // Turn on
                }
            }
        }
    }
    setCornersOn(newGrid);
    return newGrid;
}

int main() {
    std::vector<std::string> grid;
    std::string line;
     // Read the initial grid configuration from stdin
    while (std::getline(std::cin, line)) {
        grid.push_back(line);
    }

    setCornersOn(grid); // Set initial corners to on.

    // Perform 100 animation steps
    for (int i = 0; i < 100; ++i) {
       grid = performStep(grid);
    }

    // Count the number of lights that are on
    int lightsOn = 0;
    for (const auto& row : grid) {
        lightsOn += std::count(row.begin(), row.end(), '#');
    }

    // Output the final count of lit lights
    std::cout << lightsOn << std::endl;

    return 0;
}

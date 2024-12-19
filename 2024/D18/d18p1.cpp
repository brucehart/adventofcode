#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <utility>

// Constants defining the grid size
const int GRID_MAX_X = 70;
const int GRID_MAX_Y = 70;
const int GRID_SIZE_X = GRID_MAX_X + 1;
const int GRID_SIZE_Y = GRID_MAX_Y + 1;
const int MAX_BYTES = 1024;

// Structure to represent a position in the grid along with the number of steps taken to reach it
struct Position {
    int x;
    int y;
    int steps;
};

// Function to check if a given position is within the grid boundaries
bool is_within_grid(int x, int y) {
    return x >= 0 && x <= GRID_MAX_X && y >= 0 && y <= GRID_MAX_Y;
}

int main() {
    // Initialize the grid with false indicating all cells are initially safe
    std::vector<std::vector<bool>> corrupted(GRID_SIZE_X, std::vector<bool>(GRID_SIZE_Y, false));

    std::string line;
    int byte_count = 0;

    // Read input lines until EOF or until MAX_BYTES bytes are processed
    while (std::getline(std::cin, line) && byte_count < MAX_BYTES) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string token;
        int x, y;

        // Parse the X coordinate
        if (std::getline(ss, token, ',')) {
            try {
                x = std::stoi(token);
            } catch (...) {
                continue; // Skip invalid lines
            }
        } else {
            continue; // Skip invalid lines
        }

        // Parse the Y coordinate
        if (std::getline(ss, token, ',')) {
            try {
                y = std::stoi(token);
            } catch (...) {
                continue; // Skip invalid lines
            }
        } else {
            continue; // Skip invalid lines
        }

        // Ensure the coordinates are within the grid boundaries
        if (is_within_grid(x, y)) {
            corrupted[x][y] = true;
            byte_count++;
        }
    }

    // Check if the starting or ending positions are corrupted
    if (corrupted[0][0] || corrupted[GRID_MAX_X][GRID_MAX_Y]) {
        std::cout << -1;
        return 0;
    }

    // Initialize the visited grid to keep track of visited positions
    std::vector<std::vector<bool>> visited(GRID_SIZE_X, std::vector<bool>(GRID_SIZE_Y, false));
    visited[0][0] = true;

    // Initialize the BFS queue and enqueue the starting position
    std::queue<Position> q;
    q.push(Position{0, 0, 0});

    // Directions representing up, down, left, and right movements
    const std::vector<std::pair<int, int>> directions = {
        {0, 1},  // Down
        {1, 0},  // Right
        {0, -1}, // Up
        {-1, 0}  // Left
    };

    // Perform BFS to find the shortest path
    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        // Check if the exit has been reached
        if (current.x == GRID_MAX_X && current.y == GRID_MAX_Y) {
            std::cout << current.steps << std::endl;
            return 0;
        }

        // Explore all possible directions
        for (const auto& dir : directions) {
            int new_x = current.x + dir.first;
            int new_y = current.y + dir.second;

            // Check if the new position is within the grid, not corrupted, and not visited
            if (is_within_grid(new_x, new_y) && !corrupted[new_x][new_y] && !visited[new_x][new_y]) {
                visited[new_x][new_y] = true;
                q.push(Position{new_x, new_y, current.steps + 1});
            }
        }
    }

    // If the exit is not reachable, output -1
    std::cout << -1 << std::endl;
    return 0;
}

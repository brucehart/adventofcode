#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <utility>

// Constants defining the grid size
const int GRID_MAX_X = 70;
const int GRID_MAX_Y = 70;

// Structure to represent a position in the grid along with the number of steps taken
struct Position {
    int x;
    int y;
};

// Function to check if a given position is within the grid boundaries
bool is_within_grid(int x, int y) {
    return x >= 0 && x <= GRID_MAX_X && y >= 0 && y <= GRID_MAX_Y;
}

// Function to perform BFS and check if a path exists from (0,0) to (GRID_MAX_X, GRID_MAX_Y)
bool is_path_available(const std::vector<std::vector<bool>>& corrupted) {
    // If start or end is corrupted, no path exists
    if (corrupted[0][0] || corrupted[GRID_MAX_X][GRID_MAX_Y]) {
        return false;
    }

    // Directions representing up, down, left, and right movements
    const std::vector<std::pair<int, int>> directions = {
        {0, 1},  // Down
        {1, 0},  // Right
        {0, -1}, // Up
        {-1, 0}  // Left
    };

    // Initialize the visited grid
    std::vector<std::vector<bool>> visited(GRID_MAX_X + 1, std::vector<bool>(GRID_MAX_Y + 1, false));
    std::queue<Position> q;
    q.push(Position{0, 0});
    visited[0][0] = true;

    // Perform BFS
    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        // Check if the exit has been reached
        if (current.x == GRID_MAX_X && current.y == GRID_MAX_Y) {
            return true;
        }

        // Explore all possible directions
        for (const auto& dir : directions) {
            int new_x = current.x + dir.first;
            int new_y = current.y + dir.second;

            if (is_within_grid(new_x, new_y) && !corrupted[new_x][new_y] && !visited[new_x][new_y]) {
                visited[new_x][new_y] = true;
                q.push(Position{new_x, new_y});
            }
        }
    }

    // If the exit is not reachable
    return false;
}

int main() {
    // Initialize the grid with false indicating all cells are initially safe
    std::vector<std::vector<bool>> corrupted(GRID_MAX_X + 1, std::vector<bool>(GRID_MAX_Y + 1, false));

    std::string line;

    // Read input lines until EOF
    while (std::getline(std::cin, line)) {
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
        if (!is_within_grid(x, y)) {
            continue; // Ignore out-of-bound coordinates
        }

        // Corrupt the current byte position
        corrupted[x][y] = true;

        // After corrupting this byte, check if a path still exists
        if (!is_path_available(corrupted)) {
            // If no path exists, output the current byte's coordinates and terminate
            std::cout << x << "," << y << std::endl;
            return 0;
        }
    }

    // If all bytes are processed and a path still exists, no byte blocks the path
    // Depending on the problem's constraints, you might want to output something here
    // For this problem, it's assumed that a blocking byte exists, so no action is needed
    return 0;
}


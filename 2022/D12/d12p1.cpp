#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

// Structure to represent a position in the grid along with the step count
struct Position {
    int row;
    int col;
    int steps;
};

int main() {
    std::vector<std::string> grid;
    std::string line;
    
    // Read the grid from standard input
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            grid.push_back(line);
        }
    }
    
    if (grid.empty()) {
        std::cerr << "Empty grid input." << std::endl;
        return 1;
    }
    
    int numRows = grid.size();
    int numCols = grid[0].size();
    
    // Variables to store the start and end positions
    Position start{0, 0, 0};
    Position end{0, 0, 0};
    bool startFound = false, endFound = false;
    
    // Find the start (S) and end (E) positions
    for (int r = 0; r < numRows && (!startFound || !endFound); ++r) {
        for (int c = 0; c < numCols && (!startFound || !endFound); ++c) {
            if (grid[r][c] == 'S') {
                start = Position{r, c, 0};
                startFound = true;
                // Replace 'S' with 'a' for elevation purposes
                grid[r][c] = 'a';
            }
            if (grid[r][c] == 'E') {
                end = Position{r, c, 0};
                endFound = true;
                // Replace 'E' with 'z' for elevation purposes
                grid[r][c] = 'z';
            }
        }
    }
    
    if (!startFound || !endFound) {
        std::cerr << "Start or End position not found in the grid." << std::endl;
        return 1;
    }
    
    // Directions: up, down, left, right
    const std::vector<std::pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    // 2D vector to keep track of visited positions
    std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));
    visited[start.row][start.col] = true;
    
    // Initialize the queue for BFS and enqueue the start position
    std::queue<Position> q;
    q.push(start);
    
    // Perform BFS
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        
        // Check if we've reached the end
        if (current.row == end.row && current.col == end.col) {
            std::cout << current.steps << std::endl;
            return 0;
        }
        
        // Explore all possible directions
        for (const auto& dir : directions) {
            int newRow = current.row + dir.first;
            int newCol = current.col + dir.second;
            
            // Check boundaries
            if (newRow < 0 || newRow >= numRows || newCol < 0 || newCol >= numCols) {
                continue;
            }
            
            // Check if already visited
            if (visited[newRow][newCol]) {
                continue;
            }
            
            // Check elevation constraint
            char currentElevation = grid[current.row][current.col];
            char nextElevation = grid[newRow][newCol];
            if (static_cast<int>(nextElevation) <= static_cast<int>(currentElevation) + 1) {
                // Mark as visited and enqueue the new position
                visited[newRow][newCol] = true;
                q.push(Position{newRow, newCol, current.steps + 1});
            }
        }
    }
    
    // If the end is not reachable
    std::cout << "No path found" << std::endl;
    return 0;
}


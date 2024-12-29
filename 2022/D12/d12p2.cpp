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
    
    // Variables to store the end position
    Position endPos{0, 0, 0};
    bool endFound = false;
    
    // Find the end (E) position
    for (int r = 0; r < numRows && !endFound; ++r) {
        for (int c = 0; c < numCols && !endFound; ++c) {
            if (grid[r][c] == 'E') {
                endPos = Position{r, c, 0};
                endFound = true;
                // Replace 'E' with 'z' for elevation purposes
                grid[r][c] = 'z';
            }
        }
    }
    
    if (!endFound) {
        std::cerr << "End position 'E' not found in the grid." << std::endl;
        return 1;
    }
    
    // Directions: up, down, left, right
    const std::vector<std::pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    // 2D vector to keep track of visited positions
    std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));
    visited[endPos.row][endPos.col] = true;
    
    // Initialize the queue for BFS and enqueue the end position
    std::queue<Position> q;
    q.push(endPos);
    
    // 2D vector to store the number of steps to reach each position from 'E'
    std::vector<std::vector<int>> stepsGrid(numRows, std::vector<int>(numCols, -1));
    stepsGrid[endPos.row][endPos.col] = 0;
    
    // Perform BFS
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        
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
            
            char currentElevation = grid[current.row][current.col];
            char neighborElevation = grid[newRow][newCol];
            
            /*
                Reverse the elevation constraint:
                In the original problem, you can move to a square if its elevation
                is at most one higher than the current square.
                Reversing the BFS, you can move to a square if its elevation is
                at least one lower than the current square.
                This ensures that paths are correctly traced back from 'E' to all possible 'a' squares.
            */
            if (static_cast<int>(neighborElevation) >= static_cast<int>(currentElevation) - 1) {
                // Mark as visited and enqueue the new position with incremented steps
                visited[newRow][newCol] = true;
                stepsGrid[newRow][newCol] = current.steps + 1;
                q.push(Position{newRow, newCol, current.steps + 1});
            }
        }
    }
    
    // Find the minimum steps among all positions with elevation 'a'
    int minSteps = -1;
    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c) {
            if (grid[r][c] == 'a') {
                if (stepsGrid[r][c] != -1) { // If the position was reachable
                    if (minSteps == -1 || stepsGrid[r][c] < minSteps) {
                        minSteps = stepsGrid[r][c];
                    }
                }
            }
        }
    }
    
    if (minSteps != -1) {
        std::cout << minSteps << std::endl;
    } else {
        std::cout << "No path found" << std::endl;
    }
    
    return 0;
}


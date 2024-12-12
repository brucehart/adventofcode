#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

// Represents possible movement directions: up, left, down, right
const std::vector<std::pair<int, int>> DIRECTIONS = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

// Calculates the total area and perimeter of connected regions in a 2D grid
class GardenRegionAnalyzer {
private:
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<unsigned char>> fence;

    // Depth-first search to mark visited cells and track fence boundaries
    int calculateRegionArea(int row, int col, char plantType) {
        if (row < 0 || col < 0 || row >= grid.size() || col >= grid[0].size() || 
            visited[row][col] || grid[row][col] != plantType) {
            return 0;
        }

        visited[row][col] = true;
        int area = 1;

        // Check adjacent cells and mark fence boundaries
        for (size_t k = 0; k < DIRECTIONS.size(); ++k) {
            int newRow = row + DIRECTIONS[k].first;
            int newCol = col + DIRECTIONS[k].second;
            
            // Mark fence if out of bounds or different plant type
            bool outOfBounds = (newRow < 0 || newCol < 0 || 
                                newRow >= grid.size() || newCol >= grid[0].size());
            if (outOfBounds || grid[newRow][newCol] != plantType) {
                fence[row][col] |= (1 << k);
            } else if (!visited[newRow][newCol]) {
                area += calculateRegionArea(newRow, newCol, plantType);
            }
        }
        return area;
    }

    // Resolve and count fence lines
    int resolveFencePerimeter() {
        int perimeter = 0;
        
        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[0].size(); ++j) {
                for (int k = 0; k < 4; ++k) {
                    if (fence[i][j] & (1 << k)) {
                        // Found a fence line start
                        perimeter++;
                        
                        // Determine line traversal direction
                        std::pair<int, int> lineDir = (k == 0 || k == 2) ? 
                            std::make_pair(0, 1) :  // Horizontal line
                            std::make_pair(1, 0);  // Vertical line

                        int tmpi = i, tmpj = j;
                        while (tmpi >= 0 && tmpj >= 0 && 
                               tmpi < grid.size() && tmpj < grid[0].size() && 
                               (fence[tmpi][tmpj] & (1 << k))) {
                            fence[tmpi][tmpj] &= ~(1 << k);
                            tmpi += lineDir.first;
                            tmpj += lineDir.second;
                        }
                    }
                }
            }
        }
        return perimeter;
    }

public:
    // Read input from stdin into a grid
    void readInput() {
        std::string line;
        while (std::getline(std::cin, line) && !line.empty()) {
            grid.push_back(std::vector<char>(line.begin(), line.end()));
        }

        if (grid.empty()) {
            return;
        }

        // Initialize visited and fence grids
        visited = std::vector<std::vector<bool>>(
            grid.size(), std::vector<bool>(grid[0].size(), false));
        fence = std::vector<std::vector<unsigned char>>(
            grid.size(), std::vector<unsigned char>(grid[0].size(), 0));
    }

    // Calculate total score (sum of area * perimeter for each region)
    long long calculateTotalScore() {
        if (grid.empty()) return 0;

        long long totalScore = 0;

        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[0].size(); ++j) {
                if (!visited[i][j]) {
                    // Reset fence for each new region
                    std::fill(fence.begin(), fence.end(), 
                              std::vector<unsigned char>(grid[0].size(), 0));
                    
                    // Calculate region area and find its perimeter
                    int area = calculateRegionArea(i, j, grid[i][j]);
                    int perimeter = resolveFencePerimeter();
                    
                    totalScore += static_cast<long long>(area) * perimeter;
                }
            }
        }

        return totalScore;
    }
};

int main() {
    // Optimize input/output operations
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    GardenRegionAnalyzer analyzer;
    analyzer.readInput();
    
    std::cout << analyzer.calculateTotalScore() << std::endl;

    return 0;
}
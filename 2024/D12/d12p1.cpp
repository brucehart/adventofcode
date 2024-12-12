#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>

// Represents a cell in the grid with its plant type and visited status
class GridCell {
public:
    char plantType;
    bool visited;
    int regionId;

    explicit GridCell(char type) : 
        plantType(type), 
        visited(false), 
        regionId(-1) {}
};

// Manages the grid analysis and region calculation
class RegionAnalyzer {
private:
    // Static directions for grid traversal (up, down, left, right)
    static constexpr std::array<std::pair<int, int>, 4> DIRECTIONS = {{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    }};

    std::vector<std::vector<GridCell>> grid;

    // Find and mark a connected region of cells with the same plant type
    std::vector<std::pair<int, int>> findRegion(int startRow, int startCol) {
        std::vector<std::pair<int, int>> regionCells;
        std::vector<std::pair<int, int>> stack{{startRow, startCol}};
        char targetPlantType = grid[startRow][startCol].plantType;
        int currentRegionId = grid[startRow][startCol].regionId;

        while (!stack.empty()) {
            auto [row, col] = stack.back();
            stack.pop_back();

            // Mark the current cell as part of the region
            grid[row][col].visited = true;
            regionCells.emplace_back(row, col);

            // Explore adjacent cells
            for (const auto& [dx, dy] : DIRECTIONS) {
                int newRow = row + dx;
                int newCol = col + dy;

                // Check bounds and unvisited cells with same plant type
                if (isValidCell(newRow, newCol) && 
                    !grid[newRow][newCol].visited && 
                    grid[newRow][newCol].plantType == targetPlantType) {
                    
                    stack.emplace_back(newRow, newCol);
                    grid[newRow][newCol].visited = true;
                    grid[newRow][newCol].regionId = currentRegionId;
                }
            }
        }

        return regionCells;
    }

    // Calculate perimeter of a region
    int calculateRegionPerimeter(const std::vector<std::pair<int, int>>& regionCells) {
        int perimeter = 0;
        
        for (const auto& [row, col] : regionCells) {
            int adjacentCellCount = 0;

            // Count adjacent cells within the same region
            for (const auto& [dx, dy] : DIRECTIONS) {
                int newRow = row + dx;
                int newCol = col + dy;

                if (isValidCell(newRow, newCol) && 
                    grid[newRow][newCol].regionId == grid[row][col].regionId) {
                    adjacentCellCount++;
                }
            }

            // Perimeter is number of edges not connected to region cells
            perimeter += (4 - adjacentCellCount);
        }

        return perimeter;
    }

    // Check if cell is within grid bounds
    bool isValidCell(int row, int col) const {
        return row >= 0 && row < grid.size() && 
               col >= 0 && col < grid[row].size();
    }

public:
    // Read input grid from standard input
    void readInput() {
        std::string line;
        while (std::getline(std::cin, line) && !line.empty()) {
            std::vector<GridCell> row;
            std::transform(line.begin(), line.end(), std::back_inserter(row), 
                           [](char c) { return GridCell(c); });
            grid.push_back(row);
        }
    }

    // Calculate total cost by analyzing regions
    long long calculateTotalCost() {
        if (grid.empty()) return 0;

        long long totalCost = 0;
        int regionCount = 0;

        for (int row = 0; row < grid.size(); ++row) {
            for (int col = 0; col < grid[row].size(); ++col) {
                if (!grid[row][col].visited) {
                    // Assign a unique region ID
                    grid[row][col].regionId = ++regionCount;

                    // Find all cells in this region
                    std::vector<std::pair<int, int>> regionCells = 
                        findRegion(row, col);

                    // Calculate region's contribution to total cost
                    int area = regionCells.size();
                    int perimeter = calculateRegionPerimeter(regionCells);
                    totalCost += static_cast<long long>(area) * perimeter;
                }
            }
        }

        return totalCost;
    }
};

int main() {
    // Optimize input/output operations
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    RegionAnalyzer analyzer;
    analyzer.readInput();
    
    std::cout << analyzer.calculateTotalCost() << std::endl;

    return 0;
}
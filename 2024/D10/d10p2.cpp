#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Represents a 2D point with row and column coordinates
struct Point {
    int row;
    int col;

    Point(int r, int c) : row(r), col(c) {}

    // Equality comparison for Point
    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }
};

// Represents the topographic map
struct TopoMap {
    std::vector<std::vector<int>> grid;

    // Returns the number of rows in the map
    int rows() const {
        return grid.size();
    }

    // Returns the number of columns in the map
    int cols() const {
        return grid.empty() ? 0 : grid[0].size();
    }

    // Adds a new row to the grid from a string
    void insertRow(const std::string& row) {
        std::vector<int> newRow(row.begin(), row.end());
        grid.push_back(newRow);
    }

    // Retrieves the value at the specified row and column
    char get(int row, int col) const {
        if (row < 0 || row >= rows() || col < 0 || col >= cols()) {
            return '\0'; // Return null character for out-of-bounds access
        }
        return grid[row][col];
    }

    // Overloaded get function to retrieve the value at a Point
    char get(const Point& p) const {
        return get(p.row, p.col);
    }

    // Finds all trailheads (points with value '0')
    std::vector<Point> findTrailHeads() const {
        std::vector<Point> trailHeads;

        for (int r = 0; r < rows(); ++r) {
            for (int c = 0; c < cols(); ++c) {
                if (grid[r][c] == '0') {
                    trailHeads.emplace_back(r, c);
                }
            }
        }

        return trailHeads;
    }

    // Recursively calculates the score of a trail starting from the given point
    int trailHeadScore(const Point& current) {
        char trailValue = get(current);

        // If the current point marks the end of a trail ('9'), return 1
        if (trailValue == '9') {
            return 1;
        }

        int score = 0;

        // Define potential next steps (neighbors)
        std::vector<Point> nextSteps = {
            {current.row - 1, current.col}, {current.row + 1, current.col},
            {current.row, current.col - 1}, {current.row, current.col + 1}
        };

        // Explore neighbors with the next trail value
        for (const auto& next : nextSteps) {
            if (get(next) == trailValue + 1) {
                score += trailHeadScore(next);
            }
        }

        return score;
    }
};

int main() {
    TopoMap topoMap;
    std::string line;

    // Read input lines to construct the map
    while (std::getline(std::cin, line)) {
        topoMap.insertRow(line);
    }

    // Find all trailheads
    auto trailHeads = topoMap.findTrailHeads();
    int totalScore = 0;

    // Calculate the total score for all trails starting at trailheads
    for (const auto& head : trailHeads) {
        totalScore += topoMap.trailHeadScore(head);
    }

    // Output the total score of all trails
    std::cout << totalScore << std::endl;

    return 0;
}

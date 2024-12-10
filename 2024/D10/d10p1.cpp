#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

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

// Represents a unique trail between two points
struct UniqueTrail {
    Point start;
    Point end;

    UniqueTrail(const Point& s, const Point& e) : start(s), end(e) {}

    // Equality comparison for UniqueTrail
    bool operator==(const UniqueTrail& other) const {
        return start == other.start && end == other.end;
    }

    // Comparison operator for ordering trails
    bool operator<(const UniqueTrail& other) const {
        if (start.row != other.start.row) return start.row < other.start.row;
        if (start.col != other.start.col) return start.col < other.start.col;
        if (end.row != other.end.row) return end.row < other.end.row;
        return end.col < other.end.col;
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

    // Recursively traverses the trail starting from a point
    void traverseTrail(const Point& start, const Point& current, std::set<UniqueTrail>& foundSet) {
        char trailValue = get(current);

        // Mark a completed trail when reaching a '9'
        if (trailValue == '9') {
            foundSet.emplace(start, current);
        }

        // Define potential next steps (neighbors)
        std::vector<Point> nextSteps = {
            {current.row - 1, current.col}, {current.row + 1, current.col},
            {current.row, current.col - 1}, {current.row, current.col + 1}
        };

        // Explore neighbors with the next trail value
        for (const auto& next : nextSteps) {
            if (get(next) == trailValue + 1) {
                traverseTrail(start, next, foundSet);
            }
        }
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
    std::set<UniqueTrail> uniqueTrails;

    // Traverse trails starting from each trailhead
    for (const auto& head : trailHeads) {
        topoMap.traverseTrail(head, head, uniqueTrails);
    }

    // Output the number of unique trails found
    std::cout << uniqueTrails.size() << std::endl;

    return 0;
}

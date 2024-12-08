#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>

/**
 * A simple 2D coordinate structure.
 */
struct Coordinate {
    int x;
    int y;

    bool operator<(const Coordinate& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
};

/**
 * Represents the antenna map and provides functionality
 * to determine antinodes for Part 1 of the puzzle.
 */
struct AntennaMap {
    std::vector<std::vector<int>> grid;
    std::unordered_map<char, std::set<Coordinate>> antennas; // Grouped by frequency (char)
    std::set<Coordinate> antinodes; // Unique set of antinode coordinates

    int rows() const {
        return (int)grid.size();
    }

    int cols() const {
        // Assumes a square map as per the puzzle description
        // If not guaranteed, use grid[0].size() instead.
        return (int)grid[0].size();
    }

    /**
     * Inserts a row of characters into the grid.
     */
    void insertRow(const std::string& row) {
        std::vector<int> newRow(row.begin(), row.end());
        grid.push_back(newRow);
    }

    /**
     * Identifies all antennas on the map and groups them by their frequency (character).
     */
    void addAntennas() {
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                if (grid[i][j] != '.') {
                    char freq = static_cast<char>(grid[i][j]);
                    antennas[freq].insert({i, j});
                }
            }
        }
    }

    /**
     * Counts the antinodes for Part 1.
     *
     * Part 1 logic:
     * For any two antennas with the same frequency:
     * - Compute the vector (dx, dy) from one antenna to the other.
     * - The two antinodes appear one step further in the same direction from each antenna.
     *
     * That is, if you have antennas at c1 and c2, the antinodes are:
     * - c1 + (c2 - c1) = c2 + (c2 - c1) = c2 + (dx, dy)
     * - and c2 + (c1 - c2) = c1 + (c1 - c2) = c1 + (-dx, -dy)
     *
     * Ensure these calculated positions are within the map.
     */
    int countAntinodes() {
        antinodes.clear();

        // Iterate over each frequency group
        for (const auto& [freq, coords] : antennas) {
            // If there is only one antenna of this frequency, no antinodes are formed
            if (coords.size() < 2) {
                continue;
            }

            // For each pair of antennas with the same frequency
            // (We use indexes to avoid converting to a vector unnecessarily)
            auto it1 = coords.begin();
            for (int i = 0; i < (int)coords.size(); ++i, ++it1) {
                auto c1 = *it1;
                auto it2 = it1;
                ++it2;
                for (int j = i + 1; j < (int)coords.size(); ++j, ++it2) {
                    auto c2 = *it2;

                    // Compute the forward direction (from c1 to c2)
                    int dx = c2.x - c1.x;
                    int dy = c2.y - c1.y;

                    // Antinode on the c1 -> c2 direction past c2
                    Coordinate antinode1 = {c2.x + dx, c2.y + dy};
                    if (antinode1.x >= 0 && antinode1.x < rows() &&
                        antinode1.y >= 0 && antinode1.y < cols()) {
                        antinodes.insert(antinode1);
                    }

                    // Antinode on the c2 -> c1 direction past c1 (reverse direction)
                    Coordinate antinode2 = {c1.x - dx, c1.y - dy};
                    if (antinode2.x >= 0 && antinode2.x < rows() &&
                        antinode2.y >= 0 && antinode2.y < cols()) {
                        antinodes.insert(antinode2);
                    }
                }
            }
        }

        return (int)antinodes.size();
    }
};

int main() {
    AntennaMap am;
    std::string line;

    // Read the entire input into the map
    while (std::getline(std::cin, line)) {
        am.insertRow(line);
    }

    // Identify all antennas on the grid
    am.addAntennas();

    // Compute and print the number of antinodes for Part 1
    int count = am.countAntinodes();
    std::cout << count << std::endl;

    return 0; 
}

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <unordered_set>

struct Coordinate {
    int x;
    int y;

    bool operator < (const Coordinate& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }

    bool operator == (const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
};

struct CoordinateHash {
    std::size_t operator()(const Coordinate& c) const {
        return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
    }
};

static bool onGrid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

int main() {
    std::vector<std::string> grid;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            grid.push_back(line);
        }
    }

    int n_rows = (int)grid.size();
    if (n_rows == 0) {
        std::cout << 0 << std::endl;
        return 0;
    }
    int n_cols = (int)grid[0].size();

    // Collect antennas by their frequency (character)
    std::unordered_map<char, std::vector<Coordinate>> antennas;
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            char c = grid[i][j];
            if (c != '.') {
                antennas[c].push_back({i, j});
            }
        }
    }

    // Part 2 logic:
    // For each set of antennas of the same frequency, 
    // for each pair (loc1, loc2):
    //   - Include loc1 and loc2 as antinodes.
    //   - Compute direction (dx, dy) = (loc2 - loc1).
    //   - Move backward from loc1 in the direction (-dx, -dy), adding all collinear points until out of bounds.
    //   - Move forward from loc2 in the direction (dx, dy), adding all collinear points until out of bounds.

    std::unordered_set<Coordinate, CoordinateHash> antinodes_part2;

    for (auto &kv : antennas) {
        const auto &locations = kv.second;
        if (locations.size() < 2) continue;

        for (size_t i = 0; i < locations.size(); ++i) {
            for (size_t j = i + 1; j < locations.size(); ++j) {
                auto loc1 = locations[i];
                auto loc2 = locations[j];
                int dx = loc2.x - loc1.x;
                int dy = loc2.y - loc1.y;

                // Add loc1 and loc2
                antinodes_part2.insert(loc1);
                antinodes_part2.insert(loc2);

                // Move backward from loc1
                {
                    int x = loc1.x - dx;
                    int y = loc1.y - dy;
                    while (onGrid(x, y, n_rows, n_cols)) {
                        antinodes_part2.insert({x, y});
                        x -= dx;
                        y -= dy;
                    }
                }

                // Move forward from loc2
                {
                    int x = loc2.x + dx;
                    int y = loc2.y + dy;
                    while (onGrid(x, y, n_rows, n_cols)) {
                        antinodes_part2.insert({x, y});
                        x += dx;
                        y += dy;
                    }
                }
            }
        }
    }

    // The result for part two is simply how many unique antinodes we have found
    std::cout << (int)antinodes_part2.size() << std::endl;

    return 0;
}

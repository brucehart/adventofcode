#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<char> getAdjacent(const std::vector<std::vector<char>>& grid, int row, int col) {
    static const int kDirections[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    std::vector<char> neighbors;
    neighbors.reserve(8);

    const int rows = static_cast<int>(grid.size());
    const int cols = grid.empty() ? 0 : static_cast<int>(grid[0].size());

    for (const auto& dir : kDirections) {
        const int r = row + dir[0];
        const int c = col + dir[1];
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            neighbors.push_back(grid[r][c]);
        }
    }

    return neighbors;
}

int main() {
    long long total = 0;

    std::vector<std::vector<char>> grid;
    std::string line;
    while (std::cin >> line) {
        grid.emplace_back(line.begin(), line.end());
    }

    for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
        for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
            if (grid[r][c] != '@') {
                continue;
            }

            std::vector<char> neighbors = getAdjacent(grid, r, c);
            const int adjacent_rolls = static_cast<int>(std::count_if(
                neighbors.begin(), neighbors.end(),
                [](char ch) { return ch == '@'; }));

            if (adjacent_rolls < 4) {
                ++total;
            }
        }
    }

    std::cout << total << std::endl;
}

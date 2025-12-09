#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {
const int kDirections[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1},  {1, 0},  {1, 1}
};

int countAdjacentRolls(const std::vector<std::vector<char>>& grid, int row, int col) {
    int count = 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = grid.empty() ? 0 : static_cast<int>(grid[0].size());

    for (const auto& dir : kDirections) {
        const int r = row + dir[0];
        const int c = col + dir[1];
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == '@') {
            ++count;
        }
    }

    return count;
}
}  // namespace

int main() {
    std::vector<std::vector<char>> grid;
    std::string line;
    while (std::cin >> line) {
        grid.emplace_back(line.begin(), line.end());
    }

    long long total_removed = 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = grid.empty() ? 0 : static_cast<int>(grid[0].size());

    while (true) {
        std::vector<std::pair<int, int>> to_remove;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != '@') {
                    continue;
                }

                const int adjacent_rolls = countAdjacentRolls(grid, r, c);
                if (adjacent_rolls < 4) {
                    to_remove.emplace_back(r, c);
                }
            }
        }

        if (to_remove.empty()) {
            break;
        }

        total_removed += static_cast<long long>(to_remove.size());
        for (const auto& [r, c] : to_remove) {
            grid[r][c] = 'x';
        }
    }

    std::cout << total_removed << std::endl;
}

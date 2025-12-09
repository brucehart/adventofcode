#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> grid;
    std::string line;
    std::size_t width = 0;

    while (std::getline(std::cin, line)) {
        width = std::max(width, line.size());
        grid.push_back(line);
    }

    if (grid.empty()) {
        return 0;
    }

    for (auto& row : grid) {
        if (row.size() < width) {
            row.append(width - row.size(), '.');
        }
    }

    int start_row = -1;
    int start_col = -1;
    for (std::size_t r = 0; r < grid.size(); ++r) {
        const auto pos = grid[r].find('S');
        if (pos != std::string::npos) {
            start_row = static_cast<int>(r);
            start_col = static_cast<int>(pos);
            break;
        }
    }

    if (start_row == -1) {
        std::cout << 0 << '\n';
        return 0;
    }

    std::vector<unsigned long long> active(width, 0);
    active[start_col] = 1;  // one particle to start

    for (int r = start_row + 1; r < static_cast<int>(grid.size()); ++r) {
        std::vector<unsigned long long> next(width, 0);
        for (std::size_t c = 0; c < width; ++c) {
            const auto count = active[c];
            if (count == 0) {
                continue;
            }

            if (grid[r][c] == '^') {
                if (c > 0) {
                    next[c - 1] += count;
                }
                if (c + 1 < width) {
                    next[c + 1] += count;
                }
            } else {
                next[c] += count;
            }
        }
        active.swap(next);
    }

    unsigned long long timelines = 0;
    for (auto count : active) {
        timelines += count;
    }

    std::cout << timelines << '\n';
    return 0;
}

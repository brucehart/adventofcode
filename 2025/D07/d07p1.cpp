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

    // Pad rows so every row can be indexed safely.
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
        return 0;  // no source found
    }

    std::vector<int> active;
    if (start_row + 1 < static_cast<int>(grid.size())) {
        active.push_back(start_col);
    }

    long long splits = 0;
    std::vector<char> seen(width, 0);

    // Process the grid one row at a time, carrying the set of active beam columns.
    for (int r = start_row + 1; r < static_cast<int>(grid.size()) && !active.empty(); ++r) {
        std::fill(seen.begin(), seen.end(), 0);
        std::vector<int> next;
        next.reserve(active.size() + 2);  // allow for splits

        for (int col : active) {
            if (col < 0 || col >= static_cast<int>(width)) {
                continue;
            }

            if (grid[r][col] == '^') {
                ++splits;
                const int left = col - 1;
                const int right = col + 1;

                if (left >= 0 && !seen[left]) {
                    seen[left] = 1;
                    next.push_back(left);
                }
                if (right < static_cast<int>(width) && !seen[right]) {
                    seen[right] = 1;
                    next.push_back(right);
                }
            } else if (!seen[col]) {
                seen[col] = 1;
                next.push_back(col);
            }
        }

        active.swap(next);
    }

    std::cout << splits << '\n';
    return 0;
}

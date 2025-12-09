#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) grid.push_back(line);
    }
    if (grid.empty()) return 0;

    const int rows = grid.size();
    const int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    auto height = [&](int r, int c) { return grid[r][c] - '0'; };
    auto safe = [&](int r, int c) { return r >= 0 && r < rows && c >= 0 && c < cols; };

    std::vector<int> basin_sizes;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const int h = height(r, c);
            bool low = true;
            if (safe(r - 1, c) && height(r - 1, c) <= h) low = false;
            if (safe(r + 1, c) && height(r + 1, c) <= h) low = false;
            if (safe(r, c - 1) && height(r, c - 1) <= h) low = false;
            if (safe(r, c + 1) && height(r, c + 1) <= h) low = false;

            if (!low) continue;

            int size = 0;
            std::vector<std::pair<int, int>> stack = {{r, c}};
            visited[r][c] = true;

            while (!stack.empty()) {
                auto [cr, cc] = stack.back();
                stack.pop_back();
                size++;

                const int dr[4] = {-1, 1, 0, 0};
                const int dc[4] = {0, 0, -1, 1};
                for (int i = 0; i < 4; ++i) {
                    const int nr = cr + dr[i];
                    const int nc = cc + dc[i];
                    if (!safe(nr, nc) || visited[nr][nc]) continue;
                    if (height(nr, nc) == 9) continue;
                    visited[nr][nc] = true;
                    stack.emplace_back(nr, nc);
                }
            }

            basin_sizes.push_back(size);
        }
    }

    std::sort(basin_sizes.begin(), basin_sizes.end(), std::greater<int>());
    while (basin_sizes.size() < 3) basin_sizes.push_back(1);
    long long result = 1;
    for (int i = 0; i < 3; ++i) result *= basin_sizes[i];

    std::cout << result << std::endl;
    return 0;
}

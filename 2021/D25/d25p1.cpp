#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        grid.push_back(line);
    }
    if (grid.empty()) return 0;

    const int h = static_cast<int>(grid.size());
    const int w = static_cast<int>(grid[0].size());
    for (const auto& row : grid) {
        if (static_cast<int>(row.size()) != w) return 0;
    }

    std::vector<std::string> next = grid;

    int step = 0;
    while (true) {
        step++;
        bool moved = false;

        // East-facing.
        next = grid;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (grid[y][x] != '>') continue;
                const int nx = (x + 1) % w;
                if (grid[y][nx] == '.') {
                    next[y][x] = '.';
                    next[y][nx] = '>';
                    moved = true;
                }
            }
        }

        // South-facing (based on result of east phase).
        grid.swap(next);
        next = grid;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (grid[y][x] != 'v') continue;
                const int ny = (y + 1) % h;
                if (grid[ny][x] == '.') {
                    next[y][x] = '.';
                    next[ny][x] = 'v';
                    moved = true;
                }
            }
        }

        grid.swap(next);
        if (!moved) break;
    }

    std::cout << step << std::endl;
    return 0;
}

